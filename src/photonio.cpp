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

Engine::Engine():
calibrate(false),
	eventQueue(),
	appInputState(idle),
	udpwork(ioservice),
    //serialwork(serialioservice),
	_udpserver(ioservice,&eventQueue,&ioMutex),
    //_serialserver(serialioservice,115200,"/dev/ttyS0",&eventQueue,&ioMutex),
    wii(false),
	mouseMove(false)
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
    //serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));
	

    //wii=remote.Connect(wiimote::FIRST_AVAILABLE);

    //if (wii) { 	remote.SetLEDs(0x01); }
    //else { errorLog << "WiiRemote Could not Connect \n"; }

	appInputState = idle; 
	technique = spaceNavigator;
	rotTechnique = trackBall;

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

	last_mx = last_my = cur_mx = cur_my = 0;

	consumed = false;

#define	SHADOW_MAP_RATIO 2;
}

void Engine::initResources() {
   
	//Create the perspective matrix
	projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 

	//Camera position
	//cameraPosition = vec3(0,-0.11f,3.0f); 
	cameraPosition = vec3(0,0,0); 
	//viewMatrix = glm::lookAt(cameraPosition,vec3(0,0,0),vec3(0,1,0));
	viewMatrix = glm::lookAt(cameraPosition,vec3(0,0,-1),vec3(0,1,0));

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);

	restoreRay=false; //used to make ray long again if NOT intersecting

    pointLight.position = glm::vec3(0,55,-5);
    pointLight.direction = glm::vec3(0,-1,0);
    pointLight.color = glm::vec4(1,1,1,1);

    pointLight.viewMatrix = glm::lookAt(pointLight.position,glm::vec3(0,0,-5),glm::vec3(0,0,-1));

	 //load shaders from files
	colorShader = pho::Shader("shaders/shader");  
	flatShader = pho::Shader("shaders/offscreen");
	textureShader = pho::Shader("shaders/texader");

    textureShader = pho::Shader("shaders/texader");
    shadowMapLoc = glGetUniformLocation(textureShader.program, "shadowMap");
    baseImageLoc = glGetUniformLocation(textureShader.program, "texturex");

	directionalShader = pho::Shader("shaders/specular");
	normalShader = pho::Shader("shaders/normals");

	initSimpleGeometry();

	cursor.modelMatrix = glm::translate(vec3(0,0,-5));
	plane.modelMatrix = cursor.modelMatrix;

	generateShadowFBO();
}

//checks event queue for events
//and consumes them all
void Engine::checkEvents() {
	vec2 ft;
	float newAngle;

	checkKeyboard();

	if (technique == mouse) {
		float wheel = glfwGetMouseWheel();
		if (wheel != prevMouseWheel) {
			float amount = (wheel - prevMouseWheel)/10;
			cursor.modelMatrix = glm::translate(vec3(0,0,-amount))*cursor.modelMatrix;
			prevMouseWheel = wheel;
		}
	}
	  
	if (technique == planeCasting) {
		checkUDP();

		if (appInputState == rotate && rotTechnique == pinch && (consumed == false)) {
			
			p1t = p1c-p1p;
			p2t = p2c-p2p;

			ft.x=std::max(0.0f,std::min(p1t.x,p2t.x)) + std::min(0.0f,std::max(p1t.x,p2t.x));
			ft.y=std::max(0.0f,std::min(p1t.y,p2t.y)) + std::min(0.0f,std::max(p1t.y,p1t.y));

			referenceAngle = atan2((p2p.y - p1p.y) ,(p2p.x - p1p.x)); 
			newAngle = atan2((p2c.y - p1c.y),(p2c.x - p1c.x));
			
			cursor.rotate(glm::rotate((newAngle-referenceAngle)*(-50),vec3(0,0,1)));

			cursor.rotate(glm::rotate(ft.x*150,vec3(0,1,0)));
			cursor.rotate(glm::rotate(ft.y*150,vec3(1,0,0)));

			consumed = true;
		}

	}

	if (technique == rayCasting && wii) {
		checkPolhemus();
        //checkWiiMote();
	}


	//Joystick
	checkSpaceNavigator();
}

void Engine::render() {
	CALL_GL(glClearColor(1.0f,1.0f,1.0f,1.0f));
	CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	
	shadowMapRender();

	if (restoreRay) {

		CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
		
		float d = -1000.0f;

		for (int i = 5; i < 3*rayVerticesCount; i+=6) {
			CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,i*sizeof(float),sizeof(d),&d));  //edit the 6th float, i.e. the Z
		}

		restoreRay = false;  //we already restored, no need to do it every frame
	}
	
	if (technique == mouse) {
		float intersectionDistance = -1;
		if (cursor.findSphereIntersection(rayOrigin,rayDirection,sphereIntersectionPoint,sphereIntersectionDistance,sphereIntersectionNormal)) {
			sphereHit = true; 	} else { sphereHit = false; }
	}

	if (technique == rayCasting ) {

		if (cursor.findIntersection(ray.modelMatrix,objectIntersectionPoint) && (appInputState != translate) && appInputState !=rotate) {
		//if (cursor.findIntersection(rayOrigin,rayDirection,objectIntersectionPoint)) {
			objectHit = true; //picked up by checkEvents in wii-mote mode switch

			CALL_GL(glDisable(GL_DEPTH_TEST));
			//render selection red border first with the flat color shader
			flatShader.use();
			flatShader["baseColor"] = vec4(1.0f, 0.0f ,0.0f, 0.5f);
			flatShader["mvp"] = projectionMatrix*viewMatrix*glm::scale(cursor.modelMatrix,vec3(1.1f,1.1f,1.1f));
			// draw the object's outline
			cursor.bind();
			CALL_GL(glDrawArrays(GL_TRIANGLES,0,cursor.vertices.size()));

			//Ray length calculation
			rayLength = -glm::distance(ray.getPosition(),objectIntersectionPoint);
			rayLengthObject = rayLength;
			//Shorten the beam to match the object
			CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
			for (int i = 5; i < 3*rayVerticesCount; i+=6) {
				CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,i*sizeof(float),sizeof(rayLength),&rayLength));  //edit every 6th float, i.e. the Z
			}

			restoreRay = true; //mark ray to be restored to full length
		} else {objectHit = false; }

		


		float intersectionDistance = -1;
		if (cursor.findSphereIntersection(ray.modelMatrix,sphereIntersectionPoint,sphereIntersectionDistance,sphereIntersectionNormal)) {
			sphereHit = true;

			//Ray length calculation
			rayLength = -glm::distance(ray.getPosition(),sphereIntersectionPoint);

			if(!objectHit) {
				//Shorten the beam to match the object
				CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
				for (int i = 5; i < 3*rayVerticesCount; i+=6) {
					CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,i*sizeof(float),sizeof(rayLength),&rayLength));  //edit the 6th float, i.e. the Z
				}
			}
			restoreRay = true; //mark ray to be restored to full length

		} else {sphereHit = false; }

		
	}

	CALL_GL(glEnable(GL_DEPTH_TEST));
	
	

	if (technique == planeCasting && appInputState != rotate) {
		flatShader.use(); //bind the standard shader for default colored objects
		flatShader["mvp"] = projectionMatrix*viewMatrix*plane.modelMatrix;
		flatShader["baseColor"] = vec4(0.0f, 0.5f ,1.0f, 0.6f);
		CALL_GL(glLineWidth(7.0f));
		plane.bind();
		CALL_GL(glDrawArrays(GL_LINES,0,plane.vertices.size()));
	}
   
	/*colorShader.use(); //bind the standard shader for default colored objects
	colorShader["mvp"] = projectionMatrix*viewMatrix*ray.modelMatrix;
	ray.draw(true);
	flatShader.use();
	flatShader["mvp"] = projectionMatrix*viewMatrix*ray.modelMatrix;
	flatShader["baseColor"] = vec4(1.0f, 0.5f ,1.0f, 1.0f);
	ray.draw();*/
	
	
	/// CURSOR ////////////////////////////////////////
	directionalShader.use();
	directionalShader["alpha"] = 1.0f;
	directionalShader["mvp"] = projectionMatrix*viewMatrix*cursor.modelMatrix;
	directionalShader["modelMatrix"] = cursor.modelMatrix;
	cursor.bind();	
	CALL_GL(glDrawArrays(GL_TRIANGLES,0,cursor.vertices.size()));
	
	/*normalShader.use();
	normalShader["mvp"]= projectionMatrix*viewMatrix*cursor.modelMatrix;
	normalShader["modelMatrix"] = cursor.modelMatrix;
	normalShader["viewMatrix"] = viewMatrix;
	normalShader["projectionMatrix"] = projectionMatrix;
	cursor.bind();
	CALL_GL(glPointSize(13.0));
	CALL_GL(glDrawArrays(GL_TRIANGLES,0,cursor.vertices.size()));*/

	directionalShader.use();
	directionalShader["alpha"] = 0.2f;
	directionalShader["mvp"] = projectionMatrix*viewMatrix*target.modelMatrix;
	directionalShader["modelMatrix"] = target.modelMatrix;
	target.bind();
	CALL_GL(glDrawArrays(GL_TRIANGLES,0,target.vertices.size()));
	directionalShader["alpha"] = 1.0f;
	CALL_GL(glLineWidth(2.0f));
	target.bind();
	CALL_GL(glDrawArrays(GL_LINE_STRIP,0,target.vertices.size()));

	if (technique == rayCasting ) { flatShader.use();
		flatShader["baseColor"] = vec4(0.2f, 0.4f ,1.0f, 1.0f); //back to drawing with colors
		flatShader["mvp"] = projectionMatrix*viewMatrix*ray.modelMatrix;
		ray.bind();
		CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,0,ray.vertices.size()));
	}
	/*normalShader.use();
	normalShader["mvp"] = projectionMatrix*viewMatrix*cursor.modelMatrix;
	normalShader["modelMatrix"] = cursor.modelMatrix;
	CALL_GL(glLineWidth(1.0f));
	cursor.draw();*/


	if (objectHit) {  //sign that the ray has been shortened so we hit something so we must draw
		point.modelMatrix = glm::translate(objectIntersectionPoint);
		flatShader.use();
		flatShader["baseColor"] = vec4(0.0f, 1.0f ,0.0f, 1.0f); //back to drawing with colors
		flatShader["mvp"] = projectionMatrix*viewMatrix*point.modelMatrix;
		//CALL_GL(glPointSize(13.0f));
		point.bind();
		CALL_GL(glDrawArrays(GL_TRIANGLES,0,point.vertices.size()));
	}
	if ((sphereHit || (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)) && (appInputState != translate)) {
		circle.modelMatrix[3] = cursor.modelMatrix[3];

		flatShader.use();
		flatShader["mvp"] = projectionMatrix*viewMatrix*circle.modelMatrix;
		flatShader["baseColor"] = glm::vec4(1.0f,0,0,0.5f);
		CALL_GL(glLineWidth(4.0f));
		circle.bind();
		CALL_GL(glDrawArrays(GL_LINE_STRIP,0,circle.vertices.size()));

		point.modelMatrix = glm::translate(sphereIntersectionPoint);
		flatShader.use();
		flatShader["baseColor"] = vec4(1.0f, 0.0f ,0.0f, 1.0f); //back to drawing with colors
		flatShader["mvp"] = projectionMatrix*viewMatrix*point.modelMatrix;
		//CALL_GL(glPointSize(13.0f));
		point.bind();
		CALL_GL(glDrawArrays(GL_TRIANGLES,0,point.vertices.size()));
	} 

	
    textureShader.use();

    glUniform1i(baseImageLoc, 0); //Texture unit 0 is for base images.
    glUniform1i(shadowMapLoc, 1); //Texture unit 1 is for shadow maps.

    //When rendering an objectwith this program.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    textureShader["shadowMatrix"] = biasMatrix*projectionMatrix*pointLight.viewMatrix*floorMatrix;
    textureShader["mvp"] = projectionMatrix*viewMatrix*floorMatrix;
    CALL_GL(glBindVertexArray(floorVAO));
    CALL_GL(glDrawArrays(GL_TRIANGLES,0,18));


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
	glfwGetMousePos(&cur_mx,&cur_my);


	if ((button == GLFW_MOUSE_BUTTON_1) && (state == GLFW_PRESS)) 
	{	
		float norm_x = 1.0*cur_mx/WINDOW_SIZE_X*2 - 1.0;
		float norm_y = -(1.0*cur_my/WINDOW_SIZE_Y*2 - 1.0);

		glm::vec4 mouse_clip = glm::vec4((float)cur_mx * 2 / float(WINDOW_SIZE_X) - 1, 1 - float(cur_my) * 2 / float(WINDOW_SIZE_Y),0,1);

		glm::vec4 mouse_world = glm::inverse(viewMatrix) * glm::inverse(projectionMatrix) * mouse_clip;	

		rayOrigin = glm::vec3(viewMatrix[3]);
		rayDirection = glm::normalize(glm::vec3(mouse_world)-rayOrigin);

		startDrag(rayDirection,rayOrigin);
		
		appInputState = rotate;
	}
	if ((button == GLFW_MOUSE_BUTTON_1) && (state == GLFW_RELEASE)) 
	{
		appInputState = idle;
	}
	if ((button == GLFW_MOUSE_BUTTON_2) && (state == GLFW_PRESS)) {
		
		mouseMove = true;
		prevMouseExists = true;
		prevMousePos = glm::vec2(cur_mx,cur_my);

		appInputState = translate;

	}
	if ((button == GLFW_MOUSE_BUTTON_2) && (state == GLFW_RELEASE)) {
		
		mouseMove = false;
		prevMouseExists = false;
		
		appInputState = idle;
	}
}

void Engine::mouseMoveCallback(int x, int y) {
	
	float norm_x = 1.0*x/WINDOW_SIZE_X*2 - 1.0;
	float norm_y = -(1.0*y/WINDOW_SIZE_Y*2 - 1.0);

	//glm::vec4 mouse_clip = glm::vec4((float)x * 2 / float(WINDOW_SIZE_X) - 1, 1 - float(y) * 2 / float(WINDOW_SIZE_Y),0,1);
	glm::vec4 mouse_clip = glm::vec4((float)x * 2 / float(WINDOW_SIZE_X) - 1, 1 - float(y) * 2 / float(WINDOW_SIZE_Y),-1,1);

	glm::vec4 mouse_world = glm::inverse(viewMatrix) * glm::inverse(projectionMatrix) * mouse_clip;	

	rayOrigin = glm::vec3(viewMatrix[3]);
	rayDirection = glm::normalize(glm::vec3(mouse_world)-rayOrigin);
	
	if (appInputState == rotate) {
		Drag(rayDirection,rayOrigin,viewMatrix);
	}  
	
	
	if (appInputState == translate) {
		vec2 MousePt;

		MousePt.x = x;
		MousePt.y = y;

		vec2 difference = MousePt-prevMousePos;
		difference.x /= 50;
		difference.y /= 50;
		difference.y = -difference.y;
		
		if(prevMouseExists) {
		cursor.modelMatrix = glm::translate(glm::vec3(difference,0))*cursor.modelMatrix;
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

    //_serialserver.shutDown();
    //serialioservice.stop();
    //serialThread->join();

	ioservice.stop();

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
		std::cout << "translate" << std::endl;
		break;
	case translate:
		break;
	case rotate:
		if (numberOfCursors == 1) {
			p1c.x = tcur->getX();
			p1c.y = tcur->getY();
			f1id = tcur->getCursorID();

			p1p = p1c;  //when first putting finger down there must be
						// a previous spot otherwise when the other finger moves it goes crazy

		}
		if (numberOfCursors == 2) {
			rotTechnique = pinch;
			std::cout << "pinch rotate" << '\n';
			p2c.x = tcur->getX();
			p2c.y = tcur->getY();
			f2id = tcur->getCursorID();

			p2p = p2c;   //when first putting finger down there must be
			            // a previous spot otherwise when the other finger moves it goes crazy

			consumed = true;
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
	
	short numberOfCursors = tuioClient->getTuioCursors().size();
	//std::list<TUIO::TuioCursor*> cursorList;
	//cursorList = tuioClient->getTuioCursors();


	switch (appInputState) {
	case translate:
		//********************* TRANSLATE ****************************
		tempMat = mat3(orientation);
#define TFACTOR 5
		x=(tcur->getXSpeed())/TFACTOR;
		y=(tcur->getYSpeed())/TFACTOR;
		newLocationVector = tempMat*vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
		//newLocationMatrix = glm::translate(cursor.modelMatrix,newLocationVector);  //Calculate new location by translating object by motion vector
		newLocationMatrix = glm::translate(mat4(),newLocationVector);

		plane.modelMatrix = newLocationMatrix*plane.modelMatrix;
		cursor.modelMatrix = newLocationMatrix*cursor.modelMatrix;

		break;
		  
	   //*********************   ROTATE  ****************************
	case rotate:
		switch (rotTechnique) {
		case singleAxis:
			//todo: add code for aligned axis
			break;
		case screenSpace:
			if (numberOfCursors == 1) {

				if (tcur->getCursorID() == f1id) {

				p1p = p1c;

				cursor.rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
				cursor.rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

				p1c.x = tcur->getX();
				p1c.y = tcur->getY();
			}

			if (tcur->getCursorID() == f2id) {
				
				p2p = p2c;	

				cursor.rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
				cursor.rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

				p2c.x = tcur->getX();
				p2c.y = tcur->getY();

			}	

			}
			break;
		case pinch:
			// ***  PINCH  *************************


			if (tcur->getCursorID() == f1id) {

				p1p = p1c;

				p1c.x = tcur->getX();
				p1c.y = tcur->getY();

				consumed = true;
			}

			if (tcur->getCursorID() == f2id) {
				
				p2p = p2c;	

				p2c.x = tcur->getX();
				p2c.y = tcur->getY();

				consumed = false;

			}	


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
			std::cout << "screenSpace" << '\n';
			break;
		}
	}

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void Engine::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

void Engine::initSimpleGeometry() {

	
	std::vector<vec3> vertices;
	std::vector<vec3> vertixes;
	std::vector<vec3> normals;
	std::vector<vec3> colors;
	std::vector<vec3> colorx;

	//COUNTER CLOCKWISE TRIANGLE ORDER IMPORTANT FOR glm::intersectRayTriangle!!!!!!!!!!!!!!!
	colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); //red
	colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); //red

	colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); //green
	
	colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); //blue

	
	 colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); //blue
	
	colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); colors.push_back(vec3(1.0,0.0,0.0)); //red
	
	 colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); //green
	
	 colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); colors.push_back(vec3(0.0,1.0,0.0)); //green
	
	colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); colors.push_back(vec3(0.0,0.0,1.0)); //blue
	
	colors.push_back(vec3(1.0,1.0,0.0)); colors.push_back(vec3(1.0,1.0,0.0)); colors.push_back(vec3(1.0,1.0,0.0)); //yellow
	
	colors.push_back(vec3(1.0,1.0,0.0)); colors.push_back(vec3(1.0,1.0,0.0)); colors.push_back(vec3(1.0,1.0,0.0)); //yellow
	
	colors.push_back(vec3(1.0,1.0,1.0)); colors.push_back(vec3(1.0,1.0,1.0)); colors.push_back(vec3(1.0,1.0,1.0)); //white

	//bottom?
	
	colors.push_back(vec3(1.0,1.0,1.0)); colors.push_back(vec3(1.0,1.0,1.0)); colors.push_back(vec3(1.0,1.0,1.0)); //white
	colors.push_back(vec3(0.0,0.5,1.0)); colors.push_back(vec3(0.0,0.5,1.0)); colors.push_back(vec3(0.0,0.5,1.0)); //cyan
	
	vertices.push_back(vec3(-0.5,0.5,0.5)); //1 front top left
	vertices.push_back(vec3(-0.5,-0.5,0.5)); //0 front bottom left
	vertices.push_back(vec3(0.5,-0.5,0.5)); //3 front bottom right

	vertices.push_back(vec3(0.5,-0.5,0.5)); //3 front bottom right
	vertices.push_back(vec3(0.5,0.5,0.5));  //2 front top right
	vertices.push_back(vec3(-0.5,0.5,0.5)); //1 front top left

	vertices.push_back(vec3(0.5,0.5,0.5));  //2 front top right
	vertices.push_back(vec3(0.0,1.0,0.5));	//8 roof top
	vertices.push_back(vec3(-0.5,0.5,0.5)); //1 front top left

	vertices.push_back(vec3(0.5,0.5,0.5));  //2 front top right
	vertices.push_back(vec3(0.5,-0.5,0.5)); //3 front bottom right
	vertices.push_back(vec3(0.5,-0.5,-0.5));  //7

	vertices.push_back(vec3(0.5,-0.5,-0.5));  //7
	vertices.push_back(vec3(0.5,0.5,-0.5));  //6
	vertices.push_back(vec3(0.5,0.5,0.5));  //2 front top right
	vertices.push_back(vec3(0.5,0.5,-0.5));  //6
	vertices.push_back(vec3(0.0,1.0,0.5));	//8 roof top
	vertices.push_back(vec3(0.5,0.5,0.5));  //2 front top right

	vertices.push_back(vec3(0.5,-0.5,-0.5));  //7
	vertices.push_back(vec3(-0.5,-0.5,-0.5)); //4
	vertices.push_back(vec3(0.5,0.5,-0.5));  //6

	vertices.push_back(vec3(0.5,0.5,-0.5));  //6
	vertices.push_back(vec3(-0.5,-0.5,-0.5)); //4
	vertices.push_back(vec3(-0.5,0.5,-0.5)); //5

	vertices.push_back(vec3(-0.5,0.5,-0.5)); //5
	vertices.push_back(vec3(0.0,1.0,0.5));	//8 roof top
	vertices.push_back(vec3(0.5,0.5,-0.5));  //6
	
	vertices.push_back(vec3(-0.5,-0.5,-0.5)); //4
	vertices.push_back(vec3(-0.5,-0.5,0.5)); //0 front bottom left
	vertices.push_back(vec3(-0.5,0.5,-0.5)); //5

	vertices.push_back(vec3(-0.5,0.5,-0.5)); //5
	vertices.push_back(vec3(-0.5,-0.5,0.5)); //0 front bottom left
	vertices.push_back(vec3(-0.5,0.5,0.5)); //1 front top left

	vertices.push_back(vec3(-0.5,0.5,-0.5)); //5
	vertices.push_back(vec3(-0.5,0.5,0.5)); //1 front top left
	vertices.push_back(vec3(0.0,1.0,0.5));	//8 roof top

	vertices.push_back(vec3(-0.5,-0.5,-0.5)); //4
	vertices.push_back(vec3(0.5,-0.5,0.5)); //3 front bottom right
	vertices.push_back(vec3(-0.5,-0.5,0.5)); //0 front bottom left

	vertices.push_back(vec3(-0.5,-0.5,-0.5)); //4
	vertices.push_back(vec3(0.5,-0.5,-0.5));  //7
	vertices.push_back(vec3(0.5,-0.5,0.5)); //3 front bottom right
	
	
	cursor = pho::Mesh(vertices,colors);
	
	target = pho::Mesh(vertices,colors);
	target.modelMatrix = glm::translate(glm::mat4(),glm::vec3(-2,2,-5));

	
	vertices.clear();

	vertices.push_back(vec3(-0.7,0,1));
	vertices.push_back(vec3(-0.7,0,-1));
	vertices.push_back(vec3(-0.7,0,-1));
	vertices.push_back(vec3(0.7,0,-1));
	vertices.push_back(vec3(0.7,0,-1));
	vertices.push_back(vec3(0.7,0,1));
	vertices.push_back(vec3(0.7,0,1));
	vertices.push_back(vec3(-0.7,0,1));

	colors.clear();
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));

    plane = pho::Mesh(vertices,colors);

	
	vertices.clear();
	colors.clear();

	vertices.push_back(vec3(-0.1,-0.1,0.01));
	vertices.push_back(vec3(0,0.1,0.01));
	vertices.push_back(vec3(0.1,-0.1,0.01));
	
	
	colors.push_back(vec3(0,1,0));
	colors.push_back(vec3(0,1,0));
	colors.push_back(vec3(0,1,0));
	point = pho::Mesh(vertices,colors);

	vertices.clear();
	normals.clear();
	std::vector<glm::vec2> texcoords;
	
	//FLOOOOOOOOOOOR **************************

	vertices.push_back(vec3(-1, 1,0));  //0
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3(-1,-1,0)); //1
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1,-1,0)); //2
	texcoords.push_back(glm::vec2(1,1));

	vertices.push_back(vec3( 1, 1,0)); //3
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3(-1, 1,0)); //0
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1,-1,0)); //2
	texcoords.push_back(glm::vec2(1,0));

	//calculate normals
	for (std::vector<glm::vec3>::size_type i=0; i != 6; i+=3) {
		glm::vec3 v0,v1,v2;
		v0 = vertices[i];
		v1 = vertices[i+1];
		v2 = vertices[i+2];

		glm::vec3 U,V;
		
		U = v1 - v0;
		V = v2 - v0;

		normals.push_back(glm::normalize(glm::cross(U,V)));
		normals.push_back(glm::normalize(glm::cross(U,V)));
		normals.push_back(glm::normalize(glm::cross(U,V)));
	}

    GLuint buffer;

    CALL_GL(glGenVertexArrays(1,&floorVAO));
    CALL_GL(glGenBuffers(1,&buffer));
    CALL_GL(glGenBuffers(1,&rayVBO));

    CALL_GL(glBindVertexArray(floorVAO));

    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,rayVBO));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
    CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));

    CALL_GL(glGenBuffers(1,&buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER,texcoords.size()*2*sizeof(GLfloat),texcoords.data(),GL_STATIC_DRAW));
    CALL_GL(glVertexAttribPointer(texCoordLoc,2,GL_FLOAT,GL_FALSE,0,0));
    CALL_GL(glEnableVertexAttribArray(texCoordLoc));

    CALL_GL(glGenBuffers(1,&buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER,normals.size()*3*sizeof(GLfloat),normals.data(),GL_STATIC_DRAW));
    CALL_GL(glVertexAttribPointer(normalLoc,3,GL_FLOAT,GL_TRUE,0,0));
    CALL_GL(glEnableVertexAttribArray(normalLoc));

    CALL_GL(glGenBuffers(1,&buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
    CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_TRUE,0,0));
    CALL_GL(glEnableVertexAttribArray(colorLoc));

    CALL_GL(glBindVertexArray(0));

    floorMatrix = glm::scale(glm::mat4(1.),glm::vec3(55,55,0));
    floorMatrix = glm::rotate(glm::mat4(1.),-90.0f,glm::vec3(1,0,0))*floorMatrix;
    floorMatrix = glm::rotate(glm::mat4(1.),90.0f,glm::vec3(0,1,0))*floorMatrix;
    floorMatrix = glm::translate(glm::mat4(1.),glm::vec3(0,-3,-7))*floorMatrix;

    //Texture Loading

    CALL_GL(glGenTextures(1,&floorTexture));
    floorTexture = gli::createTexture2D("assets/grid.dds");


	vertices.clear();
	colors.clear();
	normals.clear();
	texcoords.clear();

	for(float i = 0; i < 6.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi 
	{
		vertices.push_back(glm::vec3(ARCBALL_RADIUS*cos(i),ARCBALL_RADIUS*sin(i),0));
    }

    circle = pho::Mesh(vertices);
	
	//RAY Cylindrical
	vertices.clear();
	rayVerticesCount =0;
	float radius = 0.01f;

	for(float i = 0; i < 6.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi 
	{
		vertices.push_back(glm::vec3(radius*cos(i),radius*sin(i),0));		
		rayVerticesCount++;
		vertices.push_back(glm::vec3(radius*cos(i),radius*sin(i),-1000));	
		rayVerticesCount++;
	}

	ray = pho::Mesh(vertices);


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
					printf("rotate");
					appInputState = rotate; }
				else
					{ appInputState = idle; 
					printf("idle");}
				break;
			case 3:
				if (appInputState != rotate) {
					printf("rotate");
					appInputState = rotate; }
				else
					{ appInputState = idle; 
					printf("idle");}
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

		position = vec3(temp[0]/100,temp[1]/100,temp[2]/100);
		//position += vec3(0,-0.575f,2.1f);
		position += vec3(0,-0.25f,0.51f);

		orientation.w = temp[3];
		orientation.x = temp[4];
		orientation.y = temp[5];
		orientation.z = temp[6];

		//transform=glm::translate(transform,position);

		transform=glm::toMat4(orientation);
		
		//Rotate the matrix from the Polhemus tracker so that we can mount it on the wii-mote with the cable running towards the floor.
		transform = transform*glm::toMat4(glm::angleAxis(-90.0f,vec3(0,0,1)));  //multiply from the right --- WRONG!!!!!! but works for the time being

		transform[3][0] = position.x; //add position to the matrix (raw, unrotated)
		transform[3][1] = position.y;
		transform[3][2] = position.z;

		ray.modelMatrix = transform;

		rayOrigin = position;
		rayDirection = glm::mat3(transform)*glm::vec3(0,0,-1);
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
		ray.modelMatrix[3][2] -=0.1;
	}

	if (glfwGetKey(GLFW_KEY_KP_9)) {
		ray.modelMatrix[3][2] +=0.1;
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
		plane.modelMatrix = glm::translate(0,0,-5);
		viewMatrix = mat4();
	}
	if (glfwGetKey(GLFW_KEY_END)) {
		perspective -=1.0;
		projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 
		std::cout << "Perspective : " << perspective << '\n';
	}

	if (glfwGetKey('f') == GLFW_PRESS) {
		cursor.rotate(glm::rotate(0.1f,glm::vec3(0,0,1)));
	}

	if (glfwGetKey('g') == GLFW_PRESS) {
		cursor.rotate(glm::rotate(-0.1f,glm::vec3(0,0,1)));
		
	}
	
	if (glfwGetKey('1') == GLFW_PRESS) {
		technique = mouse;
		std::cout << "Mouse" << '\n';
	}
	if (glfwGetKey('2') == GLFW_PRESS) {
		technique = rayCasting;
		std::cout << "Raycasting" << '\n';
	}
	if (glfwGetKey('3') == GLFW_PRESS) {
		technique = spaceNavigator;
		std::cout << "Space Navigator" << '\n';
	}

	if (glfwGetKey('4') == GLFW_PRESS) {
		technique = planeCasting;
		std::cout << "PlaneCasting" << '\n';
	}

	if (glfwGetKey('0') == GLFW_PRESS) {
		technique = planeCasting;
		std::cout << "PlaneCasting" << '\n';
	}
}

/*void Engine::checkWiiMote() {
	//if the connection to the wii-mote was successful
	
		remote.RefreshState();

		if (appInputState == translate && remote.Button.B()) {
			//mat4 newMat = glm::translate(ray.modelMatrix,);
			cursor.setPosition(

				//ray.getPosition()+glm::mat3(ray.modelMatrix)*glm::vec3(0,0,grabbedDistance)+grabOffset
				ray.getPosition()+glm::mat3(ray.modelMatrix)*glm::vec3(0,0,grabbedDistance) +grabOffset
				
				);
		}

		if (appInputState == idle && remote.Button.B() && objectHit) {   
			appInputState = translate;
			grabbedDistance = rayLengthObject;
			grabbedVector = objectIntersectionPoint-ray.getPosition();

			//possibly costly calculation:
			grabOffset = cursor.getPosition()-objectIntersectionPoint;
			std::cout << "translate" << '\n';
		}	

		if (appInputState == translate && !remote.Button.B()) {
			appInputState = idle;
			std::cout << "idle" << '\n';
		}


		if (appInputState == translate && remote.Button.Down() && grabbedDistance < 0) {
			grabbedDistance+=0.5;
		}

		if (appInputState == translate && remote.Button.Up()) {
			grabbedDistance-=0.5;
		}

		if (appInputState == rotate && remote.Button.A()) {
			Drag(rayDirection,rayOrigin,viewMatrix);
		}

		if (appInputState == idle && remote.Button.A()) {
			
			if(startDrag(rayDirection,rayOrigin)) {
				std::cout << "rotate" << '\n';			
				appInputState = rotate;
			}
		}
		
		if (appInputState == rotate && !remote.Button.A()) {
			appInputState = idle;
			std::cout << "idle" << '\n';	
		}	
	
}*/

bool Engine::startDrag(const vec3& rayDirection, const vec3& rayOrigin) {
	vec3 tempPoint;
    vec3 tempNormal;

	float tempFloat;
    if (cursor.findSphereIntersection(rayOrigin,rayDirection,tempPoint,tempFloat,tempNormal)) {
		previousVector = glm::normalize(glm::vec3(cursor.modelMatrix[3])-tempPoint);
		return true;
	}
	else return false;

}

void Engine::Drag(const vec3& rayDirection, const vec3& rayOrigin, glm::mat4 viewMatrix) {
	glm::vec3 currentVector;
	glm::vec3 tempPoint;
     vec3 tempNormal;
	float tempFloat;

    if (cursor.findSphereIntersection(rayOrigin,rayDirection,tempPoint,tempFloat,tempNormal)) {
		
		currentVector = glm::normalize(glm::vec3(cursor.modelMatrix[3])-tempPoint);

		float angle = acos(glm::min(1.0f, glm::dot(previousVector, currentVector)));
		glm::vec3 axis_in_camera_coord = glm::cross(previousVector, currentVector);
		glm::mat3 camera2object = glm::inverse(glm::mat3(viewMatrix) * glm::mat3(cursor.modelMatrix));
		glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
		cursor.modelMatrix = glm::rotate(cursor.modelMatrix,glm::degrees(angle), axis_in_object_coord);

		previousVector = currentVector;
	}
}

void Engine::generateShadowFBO()
    {
    int shadowMapWidth = WINDOW_SIZE_X * (int)SHADOW_MAP_RATIO;
    int shadowMapHeight =  WINDOW_SIZE_Y * (int)SHADOW_MAP_RATIO;

    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,shadowMapWidth,shadowMapHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0); //unbind the texture

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    { printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER)); }

    glClearDepth(1.0f); glEnable(GL_DEPTH_TEST);
    // Needed when rendering the shadow map. This will avoid artifacts.
    glPolygonOffset(1.0f, 0.0f); glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //to convert the texture coordinates to -1 ~ 1
    GLfloat biasMatrixf[] = {
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f };

    biasMatrix = glm::make_mat4(biasMatrixf);
    }

void Engine::shadowMapRender() {
    int shadowMapWidth = WINDOW_SIZE_X * (int)SHADOW_MAP_RATIO;
    int shadowMapHeight =  WINDOW_SIZE_Y * (int)SHADOW_MAP_RATIO;

    // Rendering into the shadow texture.
    glActiveTexture(GL_TEXTURE0);
    CALL_GL(glBindTexture(GL_TEXTURE_2D, shadowTexture));
    // Bind the framebuffer.
    CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO));
    //Clear it
    CALL_GL(glClear(GL_DEPTH_BUFFER_BIT));
    CALL_GL(glViewport(0, 0, shadowMapWidth, shadowMapHeight));
    CALL_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
    //Render stuff
    flatShader.use();
    flatShader["baseColor"] = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    flatShader["pvm"] = projectionMatrix*pointLight.viewMatrix*cursor.modelMatrix;
    cursor.bind();
    CALL_GL(glDrawArrays(GL_TRIANGLES,0,cursor.vertices.size()));

    // Revert for the scene.
    CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    CALL_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
    CALL_GL(glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));

}


void Engine::checkSpaceNavigator() { 

#define TRSCALE 2.0f
#define RTSCALE 5.0f

	float position[6];
    unsigned char buttons[2];
	std::fill_n(position,6,0.0f);

	if (glfwGetJoystickPos( GLFW_JOYSTICK_1, position,6) == 6 ) {
        glfwGetJoystickButtons(GLFW_JOYSTICK_1,buttons,2);
        if (buttons[0] == GLFW_PRESS) {

            viewMatrix = glm::translate(vec3(-1*position[0]*TRSCALE,0,0))*viewMatrix;
            viewMatrix = glm::translate(vec3(0,position[2]*TRSCALE,0))*viewMatrix;
            viewMatrix = glm::translate(vec3(0,0,position[1]*TRSCALE))*viewMatrix;
            viewMatrix = glm::rotate(RTSCALE*-1*position[5],glm::vec3(0,1,0))*viewMatrix;
            viewMatrix = glm::rotate(RTSCALE*-1*position[4],glm::vec3(0,0,1))*viewMatrix;
            viewMatrix = glm::rotate(RTSCALE*position[3],glm::vec3(1,0,0))*viewMatrix;
        }
        else {
            cursor.modelMatrix = glm::translate(vec3(position[0]*TRSCALE,0,0))*cursor.modelMatrix;
            cursor.modelMatrix = glm::translate(vec3(0,-1*position[2]*TRSCALE,0))*cursor.modelMatrix;
            cursor.modelMatrix = glm::translate(vec3(0,0,-1*position[1]*TRSCALE))*cursor.modelMatrix;
            cursor.rotate(glm::rotate(RTSCALE*position[5],glm::vec3(0,1,0)));
            cursor.rotate(glm::rotate(RTSCALE*position[4],glm::vec3(0,0,1)));
            cursor.rotate(glm::rotate(RTSCALE*-1*position[3],glm::vec3(1,0,0)));
        }
	}

}
