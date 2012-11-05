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
	_serialserver(serialioservice,115200,"COM5",&eventQueue,&ioMutex),
	wii(false),
	cursorArcBall((float)WINDOW_SIZE_X,(float)WINDOW_SIZE_Y),
	prevMouseExists(false)
{
#define SIZE 30                     //Size of the moving average filter
	accelerometerX.set_size(SIZE);  //Around 30 is good performance without gyro
	accelerometerY.set_size(SIZE);
	accelerometerZ.set_size(SIZE);
	magnetometerX.set_size(SIZE);
	magnetometerY.set_size(SIZE);
	magnetometerZ.set_size(SIZE);

	errorLog.open("error.log",std::ios_base::app);

	tuioClient = new TuioClient(3333);
	tuioClient->addTuioListener(this);
	tuioClient->connect();

	if (!tuioClient->isConnected()) {
		std::cout << "Tuio client connection failed" << std::endl;
	}

	verbose = false;

	//Protobuf custom protocol listener
	netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));
	//Polhemus
	serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));

	wii=remote.Connect(wiimote::FIRST_AVAILABLE);

	if (wii) { 	remote.SetLEDs(0x01); }
	else { errorLog << "WiiRemote Could not Connect \n"; }

	appInputState = idle; 
	appMode = rayCasting;
	rotTechnique = screenSpace;

	prevMouseWheel = 0;
	gyroData = false;
	objectHit=false;
	sphereHit=false;

	axisChange[0][0] = 1; axisChange[0][1] =  0;  axisChange[0][2] =  0;
	axisChange[1][0] = 0; axisChange[1][1] =  0;  axisChange[1][2] =  1;
	axisChange[2][0] = 0; axisChange[2][1] =  -1;  axisChange[2][2] = 0;

	tf = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");
	deltat = -1.0f; //so as to not repeat keystrokes

	perspective = 80.0f;
}

void Engine::initResources() {

	initSimpleGeometry();

	//for lighting, not used yet
	glGenBuffers(1,&(pointLight.uniformBlockIndex)); //generate buffer and store it's location in pointLight's member variable
	glBindBuffer(GL_UNIFORM_BUFFER,pointLight.uniformBlockIndex); 
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSource), (void *)(&pointLight), GL_STATIC_DRAW);

    //load shaders from files
	colorShader = pho::Shader("shaders/shader");  
	offscreenShader = pho::Shader("shaders/offscreen");

	
	//Create the perspective matrix
	projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 

	//Camera at the origin
	cameraPosition = vec3(0,0,0); 
	viewMatrix = mat4(1);
	viewMatrix = glm::translate(cameraPosition); 

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glDepthMask(GL_TRUE);

	restoreRay=false; //make ray long again if NOT intersecting

	cursor.modelMatrix = glm::translate(vec3(0,0,-5));
	plane.modelMatrix = cursor.modelMatrix;
}

//checks event queue for events
//and consumes them all
void Engine::checkEvents() {
	
	int wheel = glfwGetMouseWheel();
	if (wheel != prevMouseWheel) {
		int amount = wheel - prevMouseWheel;
		cursor.modelMatrix = glm::translate(cursor.modelMatrix, vec3(0,0,-amount));
		prevMouseWheel = wheel;
	}

	checkUDP();
	checkPolhemus();
	checkWiiMote();
	checkKeyboard();


}

void Engine::render() {
	CALL_GL(glClearColor(0,0,0,1));
	CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	
	if (restoreRay) {

		CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
		float d = -1000.0f;
		CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,5*sizeof(float),sizeof(d),&d));  //edit the 6th float, i.e. the Z

		restoreRay = false;  //we already restored, no need to do it every frame
	}

	//If we are raycasting and there's a hit
	if (appMode == rayCasting) {
		if (cursor.findIntersection(ray.modelMatrix,objectIntersectionPoint)) {
			objectHit = true; //picked up by checkEvents in wii-mote mode switch

			CALL_GL(glDisable(GL_DEPTH_TEST));
			//render selection red border first with the flat color shader
			offscreenShader.use();
			offscreenShader["baseColor"] = vec4(1.0f, 0.0f ,0.0f, 0.6f);
			offscreenShader["mvp"] = projectionMatrix*viewMatrix*glm::scale(cursor.modelMatrix,vec3(1.1f,1.1f,1.1f));
			// draw the object
			cursor.draw();

			//Ray length calculation
			rayLength = -glm::distance(vec3(ray.getPosition()),objectIntersectionPoint);
			//Shorten the beam to match the object
			CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
			CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,5*sizeof(float),sizeof(rayLength),&rayLength));

			restoreRay = true; //mark ray to be restored to full length
		} else {objectHit = false; }

		float intersectionDistance = -1;
		if (cursor.findSphereIntersection(ray.modelMatrix,sphereIntersectionPoint,sphereIntersectionDistance,sphereIntersectionNormal)) {
			sphereHit = true;

			//Ray length calculation
			rayLength = -glm::distance(vec3(ray.getPosition()),sphereIntersectionPoint);

			if(!objectHit) {
			//Shorten the beam to match the object
			CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
			CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,5*sizeof(float),sizeof(rayLength),&rayLength));
			}
			restoreRay = true; //mark ray to be restored to full length

		} else {sphereHit = false; }
	}

	CALL_GL(glEnable(GL_DEPTH_TEST));
	
	colorShader.use(); //bind the standard shader for default colored objects

	if (appMode == planeCasting) {
	colorShader["mvp"] = projectionMatrix*viewMatrix*plane.modelMatrix;
    plane.draw(true);
	}
    //cursor.draw();
	colorShader["mvp"] = projectionMatrix*viewMatrix*ray.modelMatrix;
    ray.draw(true);
	colorShader["mvp"] = projectionMatrix*viewMatrix*cursor.modelMatrix;
	cursor.draw();		
	
	//colorShader["mvp"] = projectionMatrix*viewMatrix*target.modelMatrix;
	//target.draw();

	if (objectHit) {  //sign that the ray has been shortened so we hit something so we must draw
		point.modelMatrix = glm::translate(objectIntersectionPoint);
		offscreenShader.use();
		offscreenShader["baseColor"] = vec4(0.0f, 1.0f ,0.0f, 1.0f); //back to drawing with colors
		offscreenShader["mvp"] = projectionMatrix*viewMatrix*point.modelMatrix;
		point.draw();
	}
	if (sphereHit) {
		point.modelMatrix = glm::translate(sphereIntersectionPoint);
		offscreenShader.use();
		offscreenShader["baseColor"] = vec4(1.0f, 0.0f ,0.0f, 1.0f); //back to drawing with colors
		offscreenShader["mvp"] = projectionMatrix*viewMatrix*point.modelMatrix;
		point.draw();
	}

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
	vec2 MousePt;
	int x,y;
	glfwGetMousePos(&x,&y);

	MousePt.x = x;
	MousePt.y = y;

	if ((button == 0) && (state == GLFW_PRESS)) 
	{	
		cursorArcBall.click(&MousePt);
	}
	if ((button == 0) && (state == GLFW_RELEASE)) 
	{
		//cursorArcBall.endDrag(unprojected);
	}
	if ((button == GLFW_MOUSE_BUTTON_2)) {
		prevMouseExists = true;
		prevMousePos.x = x;
		prevMousePos.y = -y;
	}
}

void Engine::mouseMoveCallback(int xpos, int ypos) {
	vec2 MousePt;
	MousePt.x = xpos;
	

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		MousePt.y = ypos;
		cursorArcBall.drag(&MousePt,&quaternion);
		//cursor.modelMatrix = cursor.modelMatrix*glm::toMat4(quaternion);
		vec4 temp = cursor.modelMatrix[3];
		cursor.modelMatrix = glm::toMat4(quaternion)*cursor.modelMatrix;
		cursor.modelMatrix[3] = temp;
	}  
	
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		
		
		MousePt.y = -ypos;
		vec2 difference = MousePt-prevMousePos;
		difference.x /= 10;
		difference.y /= 10;
		if(prevMouseExists) {
		cursor.modelMatrix = glm::translate(cursor.modelMatrix,glm::vec3(difference,0));
		}
		prevMousePos = MousePt;
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
	_serialserver.shutDown();
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
			tempOrigin = vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
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
			break;
		}
	}
	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void Engine::updateTuioCursor(TuioCursor *tcur) {
	vec3 newLocationVector;
	float x,y;
	x = tcur->getX();
	y = tcur->getY();
	mat3 tempMat;
	mat4 newLocationMatrix;
	mat4 rotation;
	tempOrigin = vec3(cursor.modelMatrix[3][0],cursor.modelMatrix[3][1],cursor.modelMatrix[3][2]);
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
		tempMat = mat3(orientation);
#define TFACTOR 3
		x=(tcur->getXSpeed())/TFACTOR;
		y=(tcur->getYSpeed())/TFACTOR;
		newLocationVector = tempMat*vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
		//newLocationMatrix = glm::translate(cursor.modelMatrix,newLocationVector);  //Calculate new location by translating object by motion vector
		newLocationMatrix = glm::translate(mat4(),newLocationVector);

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

	//COUNTER CLOCKWISE TRIANGLE ORDER IMPORTANT FOR glm::intersectRayTriangle!!!!!!!!!!!!!!!
	indices.push_back(1); indices.push_back(0); indices.push_back(3);  
	indices.push_back(3); indices.push_back(2); indices.push_back(1);
	indices.push_back(2); indices.push_back(8); indices.push_back(1);

	indices.push_back(2); indices.push_back(3); indices.push_back(7);
	indices.push_back(7); indices.push_back(6); indices.push_back(2);
	indices.push_back(6); indices.push_back(8); indices.push_back(2);

	indices.push_back(7); indices.push_back(6); indices.push_back(4);
	indices.push_back(6); indices.push_back(5); indices.push_back(4);
	indices.push_back(5); indices.push_back(8); indices.push_back(6);

	indices.push_back(4); indices.push_back(0); indices.push_back(5);
	indices.push_back(5); indices.push_back(0); indices.push_back(1);
	indices.push_back(5); indices.push_back(1); indices.push_back(8);

	indices.push_back(4); indices.push_back(3); indices.push_back(0);
	indices.push_back(4); indices.push_back(7); indices.push_back(3);

	vertices.push_back(vec3(-0.5,-0.5,0.5)); //front bottom left  0
	colors.push_back(vec3(1.0,0.0,0.0));
	vertices.push_back(vec3(-0.5,0.5,0.5)); //front top left 1
	colors.push_back(vec3(0.0,1.0,0.0));
	vertices.push_back(vec3(0.5,0.5,0.5));  //front top right  2
	colors.push_back(vec3(0.0,0.0,1.0));
	vertices.push_back(vec3(0.5,-0.5,0.5)); //front bottom right  3
	colors.push_back(vec3(1.0,1.0,1.0)); 

	vertices.push_back(vec3(-0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(-0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0));
	vertices.push_back(vec3(0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	
	vertices.push_back(vec3(0.0,1.0,0.5));	//roof top
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

    plane = pho::Mesh(vertices,indices,colors, true);

	vertices.clear();
	colors.clear();
	indices.clear();

	vertices.push_back(vec3(0,0,0));
	colors.push_back(vec3(1,0,0));
	vertices.push_back(vec3(0,0,-1000));
	colors.push_back(vec3(1,1,1));

	indices.push_back(0);
	indices.push_back(1);

    ray = pho::Mesh(vertices,indices,colors, true);

	indices.clear();
	vertices.clear();
	colors.clear();

	vertices.push_back(vec3(-0.1,-0.1,0.01));
	vertices.push_back(vec3(0,0.1,0.01));
	vertices.push_back(vec3(0.1,-0.1,0.01));

	//vertices.push_back(vec3(-0.1,-0.1,-0.1));
	//vertices.push_back(vec3(0,0.1,-0.1));
	//vertices.push_back(vec3(0.1,-0.1,-0.1));

	indices.push_back(0);indices.push_back(2);indices.push_back(1);
	//indices.push_back(3);indices.push_back(5);indices.push_back(4);
	colors.push_back(vec3(0,1,0));
	colors.push_back(vec3(0,1,0));
	colors.push_back(vec3(0,1,0));
	point = pho::Mesh(vertices,indices,colors,true);

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

    quad.modelMatrix = glm::scale(vec3(0.3,0.3,1));
    quad.modelMatrix = glm::translate(quad.modelMatrix,vec3(-2.2,-2.2,0));*/
}

void Engine::checkUDP() {
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
					printf("rotate-->idle");}
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
}

void Engine::checkPolhemus() {
	
	boost::mutex::scoped_lock lock(ioMutex);
	//SERIAL Queue
	while(!eventQueue.isSerialEmpty()) {
		boost::array<float,7> temp = eventQueue.serialPop();
		vec3 position;
		glm::quat orientation;
		mat4 transform;

		position = vec3(temp[0],temp[1],temp[2]);

		orientation.w = temp[3];
		orientation.x = temp[4];
		orientation.y = temp[5];
		orientation.z = temp[6];

		//transform=glm::translate(transform,position);

		transform=glm::toMat4(orientation);
		
		//Further rotate the matrix from the Polhemus tracker so that we can mount it on the wii-mote with the cable running towards the floor.
		transform = transform*glm::toMat4(glm::angleAxis(180.0f,vec3(0,1,0))); //order is important 
		transform = transform*glm::toMat4(glm::angleAxis(90.0f,vec3(0,0,1)));
		
		transform[3][0] = position.x; //add position to the matrix (raw, unrotated)
		transform[3][1] = position.y;
		transform[3][2] = position.z;

		//ray.modelMatrix = transform;
		
		/*if (wii) {  //just to debug polhemus positions
			remote.RefreshState();

			if (remote.Button.A()) {   
				std::cout << boost::posix_time::second_clock::local_time() << " - Polhemus x: " << position.x << '\t' << "y: " << position.y << '\t' << "z: " << position.z << '\n';
				errorLog << boost::posix_time::second_clock::local_time() << " - Polhemus x: " << position.x << '\t' << "y: " << position.y << '\t' << "z: " << position.z << '\n';
			}
		}*/


		//std::cout << "x : " << transform[3][0] << "\ty : " << transform[3][1] << "\tz : " << transform[3][2] << '\n';
	}
	lock.unlock();

}
void Engine::checkKeyboard() {
	#define FACTOR 0.5f
	if (glfwGetKey(GLFW_KEY_DOWN)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,-FACTOR));
	}

	if (glfwGetKey(GLFW_KEY_KP_4)) {
		ray.modelMatrix = ray.modelMatrix*glm::rotate(FACTOR,glm::vec3(0,1,0));
	}

	if (glfwGetKey(GLFW_KEY_KP_6)) {
		ray.modelMatrix = ray.modelMatrix*glm::rotate(-FACTOR,glm::vec3(0,1,0));
	}

	if (glfwGetKey(GLFW_KEY_KP_8)) {
		ray.modelMatrix = ray.modelMatrix*glm::rotate(FACTOR,glm::vec3(1,0,0));
	}

	if (glfwGetKey(GLFW_KEY_KP_5)) {
		ray.modelMatrix = ray.modelMatrix*glm::rotate(-FACTOR,glm::vec3(1,0,0));
	}

	if (glfwGetKey(GLFW_KEY_KP_7)) {
		ray.modelMatrix[3][0] -=1;
	}

	if (glfwGetKey(GLFW_KEY_KP_9)) {
		ray.modelMatrix[3][0] +=1;
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
		perspective +=1.0;
		projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 
		std::cout << "Perspective : " << perspective << '\n';
		
	}
	if (glfwGetKey(GLFW_KEY_SPACE)) {
		cursor.modelMatrix = glm::translate(0,0,-5);
	}
	if (glfwGetKey(GLFW_KEY_END)) {
		perspective -=1.0;
		projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 
		std::cout << "Perspective : " << perspective << '\n';
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

	if (glfwGetKey('4') == GLFW_PRESS) {
		appMode = rayCasting;
		std::cout << "RayCasting" << '\n';
	}
}
void Engine::checkWiiMote() {
	//if the connection to the wii-mote was successful
	if (wii) {
		remote.RefreshState();

		switch(appMode)  {
		case rayCasting:
			if (appInputState == idle && remote.Button.B() && objectHit) {   
				appInputState = translate;
				grabbedDistance = rayLength;

				//possibly costly calculation:
				grabOffset = glm::vec3(cursor.modelMatrix[3])-objectIntersectionPoint;

				mat4 newMat = glm::translate(ray.modelMatrix,vec3(0,0,grabbedDistance));

				cursor.modelMatrix[3][0] = newMat[3][0];
				cursor.modelMatrix[3][1] = newMat[3][1];
				cursor.modelMatrix[3][2] = newMat[3][2];
				std::cout << "translate" << '\n';
			}

			if (appInputState == translate && remote.Button.B()) {
			
				mat4 newMat = glm::translate(ray.modelMatrix,vec3(0,0,grabbedDistance)+grabOffset);
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
				break;
			}
			if (appInputState == rotate && !remote.Button.A()) {
				appInputState = idle;
				std::cout << "idle" << '\n';
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