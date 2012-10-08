#pragma warning(disable: 4819)
/*  Photonio Graphics Engine
Copyright (C) 2011 Nicholas Katzakis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include "photonio.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/math/special_functions/round.hpp>
#include "data.pb.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

Engine::Engine():
calibrate(false),
	eventQueue(),
	appInputState(idle),
	_udpserver(ioservice,&eventQueue,&ioMutex),
#if defined(_DEBUG)
	_serialserver(serialioservice,115200,"COM5",&eventQueue,&ioMutex),
#endif	
	wii(false)
{
#define SIZE 30                     //Size of the moving average filter
	accelerometerX.set_size(SIZE);  //Around 30 is good performance without gyro
	accelerometerY.set_size(SIZE);
	accelerometerZ.set_size(SIZE);
	magnetometerX.set_size(SIZE);
	magnetometerY.set_size(SIZE);
	magnetometerZ.set_size(SIZE);

	tuioClient = new TuioClient(3333);
	tuioClient->addTuioListener(this);
	tuioClient->connect();

	if (!tuioClient->isConnected()) {
		std::cout << "tuio client connection failed" << std::endl;
	}

	verbose = false;

	//Protobuf custom protocol listener
	netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));
#if defined(_DEBUG)
	//Polhemus
	serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));
#endif	
	count=0;

	wii=remote.Connect(wiimote::FIRST_AVAILABLE);

	if (wii) { 	remote.SetLEDs(0x01); }
	else { errorLog << "WiiRemote Could not Connect \n"; }


	appInputState = idle; 
	appMode = planeCasting;
	rotTechnique = screenSpace;

	grabbing=false;

	errorLog.open("error.log",std::ios_base::app);

	prevMouseWheel = 0;
	gyroData = false;

	axisChange[0][0] = 1; axisChange[0][1] =  0;  axisChange[0][2] =  0;
	axisChange[1][0] = 0; axisChange[1][1] =  0;  axisChange[1][2] =  1;
	axisChange[2][0] = 0; axisChange[2][1] =  -1;  axisChange[2][2] = 0;

	
}

void Engine::initResources() {

	initSimpleGeometry();

	glGenBuffers(1,&(pointLight.uniformBlockIndex)); //generate buffer and store it's location in pointLight's member variable
	glBindBuffer(GL_UNIFORM_BUFFER,pointLight.uniformBlockIndex); 
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSource), (void *)(&pointLight), GL_STATIC_DRAW);

    colorShader = pho::Shader("shaders/shader");
    //todo: Add uniforms

	//Calculate the matrices
	projectionMatrix = glm::perspective(45.0f, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); //create perspective matrix
	//projectionMatrix = glm::mat4();

	cameraPosition = glm::vec3(0,0,-25); //translate camera back (i.e. world forward)

	viewMatrix = mat4();
	viewMatrix = glm::translate(viewMatrix,cameraPosition); 

	glEnable (GL_BLEND);
	glDepthMask(GL_TRUE);

	CALL_GL(glLineWidth(3.5));

	restoreRay=false;
}

//checks event queue for events
//and co nsumes them all
void Engine::checkEvents() {

#define FACTOR 0.1
	if (glfwGetKey(GLFW_KEY_DOWN)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,-FACTOR));
	}
	if (glfwGetKey(GLFW_KEY_UP)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,FACTOR));
	}
	if (glfwGetKey(GLFW_KEY_LEFT)) {
		viewMatrix = glm::translate(viewMatrix, vec3(FACTOR,0,0));
	}
	if (glfwGetKey(GLFW_KEY_RIGHT)) {
		viewMatrix = glm::translate(viewMatrix, vec3(-FACTOR,0,0));
	}
	if (glfwGetKey(GLFW_KEY_PAGEUP)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,-FACTOR,0));	
	}
	if (glfwGetKey(GLFW_KEY_PAGEDOWN)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,FACTOR,0));
	}
	if (glfwGetKey(GLFW_KEY_HOME)) {
		cameraDirection = glm::rotate(cameraDirection, (float)0.001, vec3(0,1,0));
		viewMatrix = glm::lookAt(cameraPosition,cameraPosition+cameraDirection,glm::vec3(0,1,0));
		
	}
	if (glfwGetKey(GLFW_KEY_END)) {
	}
	if (glfwGetKey(GLFW_KEY_SPACE)) {
		plane.modelMatrix = glm::mat4();
		viewMatrix = glm::translate(glm::mat4(),cameraPosition);
	}
	if (glfwGetKey('1') == GLFW_PRESS) {
		rotTechnique = screenSpace;
		std::cout << "Screen Space Rotation" << '\n';
	}
	if (glfwGetKey('2') == GLFW_PRESS) {
		rotTechnique = singleAxis;
		std::cout << "Single Axis Rotation" << '\n';
	}
	if (glfwGetKey('3') == GLFW_PRESS) {
		rotTechnique = trackBall;
		std::cout << "Virtual Trackball Rotation" << '\n';
	}
	int wheel = glfwGetMouseWheel();
	if (wheel != prevMouseWheel) {
		int amount = wheel - prevMouseWheel;
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,amount));
		prevMouseWheel = wheel;
	}

	//UDP queue
	boost::mutex::scoped_lock lock(ioMutex);
	while (!eventQueue.isEmpty()) {
		//assign event to local temp var
		keimote::PhoneEvent tempEvent;
		tempEvent = eventQueue.pop();

		//check event to see what type it is
		switch (tempEvent.type()) {
		case keimote::ACCEL:
			accelerometerX.update(tempEvent.x());
			accelerometerY.update(tempEvent.y());
			accelerometerZ.update(tempEvent.z());

			acc.x = accelerometerX.get_result();
			acc.y = accelerometerY.get_result();
			acc.z = accelerometerZ.get_result();
			gyroData = false;
			break;
		case keimote::MAG:
			magnetometerX.update(tempEvent.x());
			magnetometerY.update(tempEvent.y());
			magnetometerZ.update(tempEvent.z());

			ma.x = magnetometerX.get_result();
			ma.y = magnetometerY.get_result();
			ma.z = magnetometerZ.get_result();
			gyroData = false;
			break;
		case keimote::GYRO:
			plane.modelMatrix[0][0] = tempEvent.m11(); plane.modelMatrix[0][1] = tempEvent.m21(); plane.modelMatrix[2][0] = tempEvent.m31();
			plane.modelMatrix[1][0] = tempEvent.m12(); plane.modelMatrix[1][1] = tempEvent.m22(); plane.modelMatrix[2][1] = tempEvent.m32();
			plane.modelMatrix[2][0] = tempEvent.m13(); plane.modelMatrix[2][1] = tempEvent.m23(); plane.modelMatrix[2][2] = tempEvent.m33();
			gyroData = true;
			break;
		case keimote::BUTTON:
			switch (tempEvent.buttontype()) {
			case 1:
				calibrate = true;
				break;
			case 2:
				if (appInputState != rotate) {
					printf("idle-->rotate");
					appInputState = rotate; }
				else
					{ appInputState = idle; 
					printf("rotate-->idle");
				}

				break;
			case 3:
				break;
			default:
				calibrate = true;
				break;
			}

		};
		if (computeRotationMatrix() && (!gyroData)) {
			plane.modelMatrix[0][0] = orientation[0][0]; plane.modelMatrix[0][1] = orientation[0][1]; plane.modelMatrix[0][2] = orientation[0][2]; 
			plane.modelMatrix[1][0] = orientation[1][0]; plane.modelMatrix[1][1] = orientation[1][1]; plane.modelMatrix[1][2] = orientation[1][2]; 
			plane.modelMatrix[2][0] = orientation[2][0]; plane.modelMatrix[2][1] = orientation[2][1]; plane.modelMatrix[2][2] = orientation[2][2]; 
		}
	}


	//SERIAL Queue
	while(!eventQueue.isSerialEmpty()) {
		boost::array<float,7> temp = eventQueue.serialPop();
		glm::vec3 position;
		glm::quat orientation;
		glm::mat4 transform;

		position = glm::vec3(temp[0],temp[1],temp[2]);

#if defined(_DEBUG)
		position.x-=25;
		position.y+=30;
		position.z-=20;
#else 
		//position.x+=0;
		position.y-=5;
		position.z-=1;

#endif

		//std::cout << "Polhemus x: " << position.x << '\t' << "y: " << position.y << '\t' << "z: " << position.z << '\n';

		orientation.w = temp[3];
		orientation.x = temp[4];
		orientation.y = temp[5];
		orientation.z = temp[6];

		glm::mat4 rot = glm::toMat4(orientation);
		glm::mat4 trans = glm::translate(glm::mat4(),position);	
		transform = glm::toMat4(glm::angleAxis(180.0f,glm::vec3(0,1,0)))*transform;
		transform = glm::toMat4(glm::angleAxis(-90.0f,glm::vec3(0,0,1)))*transform;


		transform = rot*transform;

		transform= trans*transform;
		
		ray.modelMatrix = transform;

		

		//std::cout << "x : " << transform[3][0] << "\ty : " << transform[3][1] << "\tz : " << transform[3][2] << '\n';
	}
	lock.unlock();



	//if the connection to the wii-mote was successful
	if (wii) {
		remote.RefreshState();

		switch(appMode)  {
		case rayCasting:
			if (appInputState == idle && remote.Button.B()) {   
				appInputState = translate;
				grabbedDistance = rayLength;

				mat4 newMat = glm::translate(ray.modelMatrix,glm::vec3(0,0,grabbedDistance));

				cursor.modelMatrix[3][0] = newMat[3][0];
				cursor.modelMatrix[3][1] = newMat[3][1];
				cursor.modelMatrix[3][2] = newMat[3][2];
				std::cout << "translate" << '\n';
			}
			if (appInputState == translate && remote.Button.B()) {
				mat4 newMat = glm::translate(ray.modelMatrix,glm::vec3(0,0,grabbedDistance));
				cursor.modelMatrix[3][0] = newMat[3][0];
				cursor.modelMatrix[3][1] = newMat[3][1];
				cursor.modelMatrix[3][2] = newMat[3][2];
			}
			if (appInputState == translate && remote.Button.Down() && grabbedDistance < 0) {
				grabbedDistance+=0.5;
			}
			if (appInputState == translate && remote.Button.Up()) {
				grabbedDistance-=0.5;
			}
			if (appInputState == translate && !remote.Button.B()) {
				appInputState = idle;
				std::cout << "idle" << '\n';
				break;
			}
			if (appInputState == idle && remote.Button.A()) {
				appInputState = rotate;
				rotTechnique = trackBall;

				std::cout << "trackball" << '\n';

				int xx,yy;
				
				glfwGetMousePos(&xx,&yy);
				arcBallPreviousPoint[0] = xx*1.0f;
				arcBallPreviousPoint[1] = yy*1.0f;
				tempOrigin = glm::vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
				break;
			}
//			if (appInputState == rotate && rotTechnique = trackBall && remote.Button.A()) {
				//trackball rotate
//				break;
//			}
		default:
			break;
		}
	}

}

void Engine::render() {
	CALL_GL(glClearColor(0,0,0,1));
	CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	
	//CALL_GL(glEnable(GL_DEPTH_TEST));
	
	colorShader.use(); //bind the standard shader for default colored objects

	colorShader["mvp"] = projectionMatrix*viewMatrix*plane.modelMatrix;
    plane.draw();
	colorShader["mvp"] = projectionMatrix*viewMatrix*cursor.modelMatrix;
    cursor.draw();
	colorShader["mvp"] = projectionMatrix*viewMatrix*ray.modelMatrix;
    ray.drawLines();
	colorShader["mvp"] = projectionMatrix*viewMatrix*target.modelMatrix;
    target.draw();

	glfwSwapBuffers();
}

bool Engine::computeRotationMatrix() {

	vec3 H = glm::cross(ma,acc);

	float normH = (float) glm::length(H);

	if (normH<0.1f) return false;

	float invH = 1.0f / normH;

	H *= invH;

	float invA = 1.0f / (float) glm::length(acc);

	acc *= invA;

	vec3 M = glm::cross(acc,H);

	orientation[0][0] = H.x;   orientation[0][1] = H.y;   orientation[0][2] = H.z;
	orientation[1][0] = M.x;   orientation[1][1] = M.y;   orientation[1][2] = M.z;
	orientation[2][0] = acc.x; orientation[2][1] = acc.y; orientation[2][2] = acc.z;

	orientation = glm::rotate(orientation, glm::degrees((float)M_PI/2),vec3(1,0,0));
	orientation = glm::inverse(orientation);
	orientation = glm::rotate(orientation, glm::degrees((float)M_PI/2),vec3(1,0,0));

	if (calibrate) {
		calibration = glm::inverse(orientation);
		calibrate = !calibrate;
	}

	orientation = calibration*orientation;

	return true;
}

void Engine::mouseButtonCallback(int button, int state) {
	if ((button == 0) && (state == GLFW_PRESS)) {
		int xx,yy;
		glfwGetMousePos(&xx,&yy);
		arcBallPreviousPoint[0] = xx*1.0f;
		arcBallPreviousPoint[1] = yy*1.0f;
		tempOrigin = glm::vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
	}
}

void Engine::mouseMoveCallback(int xpos, int ypos) {
	//std::cout << "movse moved x:" << xpos << "\t y:" << ypos << std::endl;
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		cursor.modelMatrix = glm::translate(cursor.modelMatrix,-tempOrigin);
		glm::mat4 rot;
		rot = pho::util::getRotation(arcBallPreviousPoint[0],arcBallPreviousPoint[1],xpos*1.0f,ypos*1.0f, false);
		cursor.modelMatrix = rot*cursor.modelMatrix;
		cursor.modelMatrix = glm::translate(cursor.modelMatrix,tempOrigin);
		arcBallPreviousPoint[0] = xpos*1.0f;
		arcBallPreviousPoint[1] = ypos*1.0f;
	}
}

void Engine::go() {

	initResources();
	while(true) {
		checkEvents();
		render();
		if(glfwGetKey(GLFW_KEY_ESC)) {
			shutdown();
			break;
		}
	}
}

void Engine::shutdown() {
	netThread->interrupt();
#if defined(_DEBUG)
	_serialserver.shutDown();
#endif
	serialThread->interrupt();
	errorLog.close();
}

void Engine::addTuioObject(TuioObject *tobj) {
	if (verbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void Engine::updateTuioObject(TuioObject *tobj) {

	if (verbose)
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
		<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;

}

void Engine::removeTuioObject(TuioObject *tobj) {

	if (verbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void Engine::addTuioCursor(TuioCursor *tcur) {

	//TUIO variables
	short numberOfCursors = tuioClient->getTuioCursors().size();
	std::list<TUIO::TuioCursor*> cursorList;
	cursorList = tuioClient->getTuioCursors();
	//std::cout << "Added cursor, Current NoOfCursors " << numberOfCursors << std::endl;

	switch (appInputState) {
	case idle:
		appInputState = translate;
		std::cout << "idle --> translate" << std::endl;
		break;
	case translate:
		/*if (numberOfCursors == 2) {
			appInputState = trackBall;

			trackedCursor = tcur;

			float x = tcur->getX();
			float y = tcur->getY();
			std::cout << "translate --> trackball" << std::endl;

			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			tempOrigin = glm::vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
		}*/
		break;
	case rotate:
		switch (rotTechnique) {
		case screenSpace:
			if (numberOfCursors == 1) {
				p1p.x = tcur->getX();
				p1p.y = tcur->getY();
				f1id = tcur->getCursorID();	
				
			}
			if (numberOfCursors == 2) {
				rotTechnique = pinch;
				std::cout << "pinch rotate" << '\n';
				p2p.x = tcur->getX();
				p2p.y = tcur->getY();
				f2id = tcur->getCursorID();
			}
			
			break;
		case trackBall:
			trackedCursorId = tcur->getCursorID();

			float x = tcur->getX();
			float y = tcur->getY();

			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			tempOrigin = glm::vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
		}
	}
	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void Engine::updateTuioCursor(TuioCursor *tcur) {
	glm::vec3 newLocationVector;
	float x,y;
	x = tcur->getX();
	y = tcur->getY();
	glm::mat3 tempMat;
	glm::mat4 newLocationMatrix;
	glm::mat4 rotation;
	tempOrigin = glm::vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
	glm::vec2 ftranslation;

	glm::vec2 f1translationVec,f2translationVec;
	glm::vec2 f1curr,f2curr;
	vec3 location;
	float newAngle;
	glm::vec2 ft;

	short numberOfCursors = tuioClient->getTuioCursors().size();
	std::list<TUIO::TuioCursor*> cursorList;
	cursorList = tuioClient->getTuioCursors();


	switch (appInputState) {
	case translate:
		//********************* TRANSLATE ****************************
		tempMat = glm::mat3(orientation);
#define TFACTOR 3
		x=(tcur->getXSpeed())/TFACTOR;
		y=(tcur->getYSpeed())/TFACTOR;
		newLocationVector = tempMat*glm::vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
		//newLocationMatrix = glm::translate(cursor.modelMatrix,newLocationVector);  //Calculate new location by translating object by motion vector
		newLocationMatrix = glm::translate(glm::mat4(),newLocationVector);

		plane.modelMatrix = newLocationMatrix*plane.modelMatrix;
		cursor.modelMatrix = newLocationMatrix*cursor.modelMatrix;

		break;
		  
	
	case rotate:
		switch (rotTechnique) {
		case singleAxis:
			//todo: add code for aligned axis
			break;
		case screenSpace:
			//********************* screenSpace  *************************
			if (numberOfCursors == 1) {

				vec3 location;
				location.x = cursor.modelMatrix[3][0];
				location.y = cursor.modelMatrix[3][1];
				location.z = cursor.modelMatrix[3][2];  

				cursor.modelMatrix[3][0] = 0;
				cursor.modelMatrix[3][1] = 0;
				cursor.modelMatrix[3][2] = 0;

				cursor.modelMatrix = glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0))*cursor.modelMatrix;
				cursor.modelMatrix = glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0))*cursor.modelMatrix;

				cursor.modelMatrix[3][0] = location.x;
				cursor.modelMatrix[3][1] = location.y;
				cursor.modelMatrix[3][2] = location.z;
			}
			break;
		case pinch:
			//********************* PINCH  *************************


			if (tcur->getCursorID() == f1id) {
				p1c.x = tcur->getX();
				p1c.y = tcur->getY();
				p1t = p1c-p1p;
			}

			if (tcur->getCursorID() == f2id) {
				p2c.x = tcur->getX();
				p2c.y = tcur->getY();
				
				p2t = p2c-p2p;				
			}

			ft.x=std::max(0.0f,std::min(p1t.x,p2t.x)) + std::min(0.0f,std::max(p1t.x,p2t.x));
			ft.y=std::max(0.0f,std::min(p1t.y,p2t.y)) + std::min(0.0f,std::max(p1t.y,p1t.y));

			referenceAngle = atan2((p2p.y - p1p.y) ,(p2p.x - p1p.x)); 
			newAngle = atan2((p2c.y - p1c.y),(p2c.x - p1c.x));
			
			location.x = cursor.modelMatrix[3][0];
			location.y = cursor.modelMatrix[3][1];
			location.z = cursor.modelMatrix[3][2];

			cursor.modelMatrix[3][0] = 0;
			cursor.modelMatrix[3][1] = 0;
			cursor.modelMatrix[3][2] = 0;

			cursor.modelMatrix = glm::rotate((newAngle-referenceAngle)*(-2),vec3(0,0,1))*cursor.modelMatrix;
			
			cursor.modelMatrix = glm::rotate(ft.x*5,vec3(0,1,0))*cursor.modelMatrix;
			cursor.modelMatrix = glm::rotate(ft.y*5,vec3(1,0,0))*cursor.modelMatrix;
				
			cursor.modelMatrix[3][0] = location.x;
			cursor.modelMatrix[3][1] = location.y;
			cursor.modelMatrix[3][2] = location.z;

			
			//update to latest values
			//referenceAngle = newAngle; ???????????? doesn't seem to make a difference
			//if (tcur->getCursorID() == f1id) {	p1p = p1c; }
			//if (tcur->getCursorID() == f2id) {	p2p = p2c; }
			break;
		case trackBall:
			//********************* TRACKBALL  *************************
			x = tcur->getX();
			y = tcur->getY();

			cursor.modelMatrix = glm::translate(cursor.modelMatrix,-tempOrigin);

			rotation = pho::util::getRotation(arcBallPreviousPoint[0],arcBallPreviousPoint[1],x,y,true);
			cursor.modelMatrix = rotation*cursor.modelMatrix;
			cursor.modelMatrix = glm::translate(cursor.modelMatrix,tempOrigin);
			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			break;
		}
	}
	if (verbose)
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
		<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
	
}

void Engine::removeTuioCursor(TuioCursor *tcur) {

	short numberOfCursors = tuioClient->getTuioCursors().size()-1;

	//std::cout << "Removed cursor, Current NoOfCursors " << numberOfCursors << std::endl;

	switch (appInputState) {
	case translate:
		appInputState = idle;
		std::cout << "translate-->idle" << std::endl;
		break;
	case trackBall:
		/*if (numberOfCursors == 2) {
			trackedCursor = tuioClient->getTuioCursors().back();
			arcBallPreviousPoint[0] = tcur->getX();
			arcBallPreviousPoint[1] = tcur->getY();
			std::cout << "trackball --> trackball" << std::endl;
		}
		if (numberOfCursors == 1) {
			trackedCursor = tuioClient->getTuioCursors().back();
			arcBallPreviousPoint[0] = tcur->getX();
			arcBallPreviousPoint[1] = tcur->getY();
			std::cout << "trackball with 1 finger" << std::endl;
		}
		if (numberOfCursors == 0) {
			appInputState = idle;
			std::cout << "trackball --> idle " << std::endl;
		}*/

		break;
	case rotate:
		switch (rotTechnique) {
		case pinch:
			rotTechnique = screenSpace;
			std::cout << "pinch --> screenSpace" << '\n';
		}
	}

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void Engine::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}


void Engine::initSimpleGeometry() {

	std::vector<GLushort> indices;
	std::vector<vec3> vertices;
	std::vector<vec3> colors;
	
	
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(3); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(1); indices.push_back(8);

	indices.push_back(3); indices.push_back(2); indices.push_back(7);
	indices.push_back(7); indices.push_back(2); indices.push_back(6);
	indices.push_back(6); indices.push_back(2); indices.push_back(8);

	indices.push_back(7); indices.push_back(4); indices.push_back(6);
	indices.push_back(6); indices.push_back(4); indices.push_back(5);
	indices.push_back(5); indices.push_back(6); indices.push_back(8);

	indices.push_back(4); indices.push_back(5); indices.push_back(0);
	indices.push_back(0); indices.push_back(5); indices.push_back(1);
	indices.push_back(1); indices.push_back(5); indices.push_back(8);

	indices.push_back(0); indices.push_back(4); indices.push_back(3);
	indices.push_back(3); indices.push_back(4); indices.push_back(7);

	vertices.push_back(vec3(-0.5,-0.5,0.5)); //front bottom left
	colors.push_back(vec3(1.0,0.0,0.0));
	vertices.push_back(vec3(-0.5,0.5,0.5)); //front top left
	colors.push_back(vec3(0.0,1.0,0.0));
	vertices.push_back(vec3(0.5,0.5,0.5));  //front top right
	colors.push_back(vec3(0.0,0.0,1.0));
	vertices.push_back(vec3(0.5,-0.5,0.5)); //front bottom right
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(-0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(-0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0));
	vertices.push_back(vec3(0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(0.0,1.0,0.5));	
	colors.push_back(vec3(1.0,1.0,1.0));

    target = pho::Mesh(vertices,indices,colors);
	cursor = pho::Mesh(vertices,indices,colors);
	vertices.clear();
	
	vertices.push_back(vec3(-0.7,0,1));
	vertices.push_back(vec3(-0.7,0,-1));
	vertices.push_back(vec3(0.7,0,-1));
	vertices.push_back(vec3(0.7,0,1));

	colors.clear();
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));

	indices.clear(); indices.push_back(0); indices.push_back(1);indices.push_back(1);indices.push_back(2);
	indices.push_back(2);indices.push_back(3);indices.push_back(3);indices.push_back(0);

    plane = pho::Mesh(vertices,indices,colors);

	vertices.clear();
	colors.clear();
	indices.clear();

	vertices.push_back(vec3(0,0,0));
	colors.push_back(vec3(1,0,0));
	vertices.push_back(vec3(0,0,-1000));
	colors.push_back(vec3(1,1,1));

	indices.push_back(0);
	indices.push_back(1);

    ray = pho::Mesh(vertices,indices,colors);

	indices.clear();
	vertices.clear();
	colors.clear();

	/*std::vector<glm::vec2> texcoords;

	vertices.push_back(vec3(-1,-1,0));
	texcoords.push_back(glm::vec2(0,0));

	vertices.push_back(vec3(-1, 1,0));
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1,-1,0));
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3( 1,-1,0));
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3(-1, 1,0));
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1, 1,0));
	texcoords.push_back(glm::vec2(1,1));

	indices.push_back(0);indices.push_back(1);indices.push_back(2);
	indices.push_back(3);indices.push_back(4);indices.push_back(5);	

    //Simple quad to Render off-screen buffer
    quad = pho::Asset::Asset(vertices,indices,texcoords,"quad");

    quad.modelMatrix = glm::scale(glm::vec3(0.3,0.3,1));
    quad.modelMatrix = glm::translate(quad.modelMatrix,glm::vec3(-2.2,-2.2,0));*/
}
