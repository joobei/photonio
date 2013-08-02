﻿#pragma warning(disable: 4819)
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

#include "uikun.h"
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
	udpwork(ioservice),
	_udpserver(ioservice,&eventQueue,&ioMutex),
    wii(false),
    appState(select),
    inputStarted(false),
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
    //Polhemus (disabled because it requires usb converter attached)
    //serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));
	

    //wii=remote.Connect(wiimote::FIRST_AVAILABLE);

    //if (wii) { 	remote.SetLEDs(0x01); }
    //else { errorLog << "WiiRemote Could not Connect \n"; }

    technique = planeCasting;
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

	last_mx = last_my = cur_mx = cur_my = 0;

	consumed = false;


#define	SHADOW_MAP_RATIO 8;
}

void Engine::initResources() {
    std::string shaderpath; //this is only needed at runtime whereas asssetpath might be needed later

    //find the path where the shaders are stored ("shaderpath" file created by cmake with configure_file)
    if (boost::filesystem::exists("shaderpath")) {
        shaderpath = readTextFile("shaderpath");
        shaderpath = shaderpath.substr(0,shaderpath.size()-1); //cmake puts a newline char
        shaderpath.append("/"); //at the end of the string
    }

    if (boost::filesystem::exists("assetpath")) { //if you're not using cmake put the shaders in the same dir as the binary
        assetpath = readTextFile("assetpath");
        assetpath = assetpath.substr(0,assetpath.size()-1); //cmake puts a newline char
        assetpath.append("/"); //at the end of the string
    }

    pointLight.position = glm::vec3(0,50,0);
    pointLight.direction = glm::vec3(0,-1,-1);
    pointLight.color = glm::vec4(1,1,1,1);

    pointLight.viewMatrix = glm::lookAt(pointLight.position,glm::vec3(0,0,-5),glm::vec3(0,0,-1));

    //Load shaders ***************************
    flatShader = pho::Shader(shaderpath+"flat");
    noTextureShader = pho::Shader(shaderpath+"notexture");
    noTextureShader.use();
    noTextureShader["view"] = viewMatrix;
    noTextureShader["light_position"] = glm::vec4(pointLight.position,1);
    noTextureShader["light_diffuse"] = pointLight.color;
    noTextureShader["light_specular"] = vec4(1,1,1,1);

    textureShader = pho::Shader(shaderpath+"texture");
    textureShader.use();
    textureShader["view"] = viewMatrix;
    textureShader["light_position"] = glm::vec4(pointLight.position,1);
    textureShader["light_diffuse"] = pointLight.color;
    textureShader["light_specular"] = vec4(1,1,1,1);

    GLuint t1Location = glGetUniformLocation(textureShader.program, "tex0");
    GLuint t2Location = glGetUniformLocation(textureShader.program, "tex1");

    glUniform1i(t1Location, 0);
    glUniform1i(t2Location, 1);

    //shadowMapLoc = glGetUniformLocation(textureShader.program, "shadowMap");
    //baseImageLoc = glGetUniformLocation(textureShader.program, "texturex");

    //Load Assets ***************************
    //cursor = pho::Asset("bump-heart.obj", &textureShader);
    cursor = pho::Asset("cursor.obj", &noTextureShader);
    cursor.modelMatrix = glm::translate(glm::mat4(),glm::vec3(0,0,-5));
    cursor.linkViewMatrices(&viewMatrix,&projectionMatrix);

    target = pho::Asset("floor.obj", &textureShader);
    target.linkViewMatrices(&viewMatrix,&projectionMatrix);

    floor = pho::Asset("floor.obj", &textureShader);
    floor.modelMatrix  = glm::translate(glm::mat4(),glm::vec3(0,-30,-60));
    floor.linkViewMatrices(&viewMatrix,&projectionMatrix);
   
    plane.setShader(&flatShader);
    plane.modelMatrix = cursor.modelMatrix;
    plane.setScale(15.0f);
    plane.linkViewMatrices(&viewMatrix,&projectionMatrix);

    heart = pho::Asset("bump-heart.obj",&textureShader);
    heart.modelMatrix = glm::translate(glm::mat4(),glm::vec3(0,0,-15));
    heart.linkViewMatrices(&viewMatrix,&projectionMatrix);

	//Create the perspective matrix
	projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 

	cameraPosition = vec3(0,0,0); 
	viewMatrix = glm::lookAt(cameraPosition,vec3(0,0,-1),vec3(0,1,0));

	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);

    //generateShadowFBO();
    initPhysics();
}

//checks event queue for events
//and consumes them all
void Engine::checkEvents() {
	checkKeyboard();

	if (technique == mouse) {
		float wheel = glfwGetMouseWheel();
		if (wheel != prevMouseWheel) {
			float amount = (wheel - prevMouseWheel)/10;
			cursor.modelMatrix = glm::translate(vec3(0,0,-amount))*cursor.modelMatrix;
			prevMouseWheel = wheel;
            inputStarted = true;
		}
	}
	  
	if (technique == planeCasting) {
		checkUDP();
	}

    if (flicker.inFlick()) {
        glm::mat4 flickTransform = flicker.dampenAndGiveMatrix(glm::mat3(plane.modelMatrix));
        plane.modelMatrix = flickTransform*plane.modelMatrix;  //translate plane
        pho::locationMatch(cursor.modelMatrix,plane.modelMatrix);  //put cursor in plane's location
        //cursor.modelMatrix = flickTransform*cursor.modelMatrix;
        //plane.modelMatrix = cursor.modelMatrix;
    }

    if (flicker.inPinchFlick()) {
        glm::mat4 flickTransform = flicker.dampenAndGivePinchMatrix();
        cursor.rotate(flickTransform);
    }

    if (flicker.inRotationFlick()){
        glm::vec2 rotation = flicker.dampenAndGiveRotationMatrix();
        cursor.rotate(glm::rotate(rotation.x*3.0f,vec3(0,1,0)));
        cursor.rotate(glm::rotate(rotation.y*3.0f,vec3(1,0,0)));
    }

	//Joystick
	checkSpaceNavigator();
}

void Engine::render() {
    //shadowMapRender();

    CALL_GL(glClearColor(0.0f,0.0f,0.0f,0.0f));
    CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    //if (!inputStarted) { heart.rotate(glm::rotate(0.1f,glm::vec3(0,1,0))); }
    floor.draw();
    cursor.draw();
    heart.draw();

    if (technique == planeCasting && appState != rotate) {
        plane.draw();
    }

	glfwSwapBuffers();

    physicsCheck();
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
		
        appState = rotate;
	}
	if ((button == GLFW_MOUSE_BUTTON_1) && (state == GLFW_RELEASE)) 
	{
        appState = rotate;
	}
	if ((button == GLFW_MOUSE_BUTTON_2) && (state == GLFW_PRESS)) {
		
		mouseMove = true;
		prevMouseExists = true;
		prevMousePos = glm::vec2(cur_mx,cur_my);

        appState = translate;

	}
	if ((button == GLFW_MOUSE_BUTTON_2) && (state == GLFW_RELEASE)) {
		
		mouseMove = false;
		prevMouseExists = false;
		
        appState = rotate;
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
	
    if (appState == rotate) {
		Drag(rayDirection,rayOrigin,viewMatrix);
	}  
	
	
    if (appState == translate) {
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

    inputStarted = true;

	//TUIO variables
	short numberOfCursors = tuioClient->getTuioCursors().size();
	std::list<TUIO::TuioCursor*> cursorList;
	cursorList = tuioClient->getTuioCursors();
	//std::cout << "Added cursor, Current NoOfCursors " << numberOfCursors << std::endl;

    //notify flick manager of a new gesture starting
    if (numberOfCursors == 1) {  flicker.newFlick(); flicker.stopPinchFlick();

    }

    //check for double click
     if (numberOfCursors == 1) {
         if (doubleClick.elapsed() < 500) {

         }
         doubleClick.restart();
     }

    switch (appState) {
	case translate:
		break;
	case rotate:
        if (numberOfCursors == 0) {
            flicker.stopFlick();
        }
		if (numberOfCursors == 1) {
			p1c.x = tcur->getX();
			p1c.y = tcur->getY();
			f1id = tcur->getCursorID();

			p1p = p1c;  //when first putting finger down there must be
						// a previous spot otherwise when the other finger moves it goes crazy

		}
		if (numberOfCursors == 2) {

            flicker.stopPinchFlick();
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
    btTransform temp;
	float x,y;
	x = tcur->getX();
	y = tcur->getY();
	mat3 tempMat;
	mat4 newLocationMatrix;
    mat4 tempMatrix;
	
    flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));

    vec2 ft;
    float newAngle;

	short numberOfCursors = tuioClient->getTuioCursors().size();
	//std::list<TUIO::TuioCursor*> cursorList;
	//cursorList = tuioClient->getTuioCursors();

    switch (appState) {
    case select:
    case translate:
		//********************* TRANSLATE ****************************
        tempMat = mat3(orientation);  //get the rotation part from the plane's matrix
#define TFACTOR 5
		x=(tcur->getXSpeed())/TFACTOR;
        y=(tcur->getYSpeed())/TFACTOR;
        //std::cout << "x: " << x;
        //std::cout << "\t\t y: " << y << '\n';
        //std::cout.flush();
        //add cursor to queue for flicking

		newLocationVector = tempMat*vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
        newLocationMatrix = glm::translate(mat4(),newLocationVector);   //Calculate new location by translating object by motion vector

        plane.modelMatrix = newLocationMatrix*plane.modelMatrix;  //translate plane
        pho::locationMatch(heart.modelMatrix,plane.modelMatrix);  //put cursor in plane's location

        temp.setFromOpenGLMatrix(glm::value_ptr(tempMatrix));
        coHeart->setWorldTransform(temp);
		break;		  
	   //*********************   ROTATE  ****************************
    case rotate:
        switch (rotTechnique) {
        case screenSpace:


            if (flicker.inFlick()) { flicker.stopFlick(); } //probably have come back from a pinch flick so need to stop the flick?? test without.

            if (tcur->getCursorID() == f1id) {

                p1p = p1c;
                cursor.rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                cursor.rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

                p1c.x = tcur->getX();
                p1c.y = tcur->getY();

                flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));
            }

            if (tcur->getCursorID() == f2id) {

                p2p = p2c;
                cursor.rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                cursor.rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

                p2c.x = tcur->getX();
                p2c.y = tcur->getY();

            }


            break;
        case pinch:
            // ***  PINCH  **********

            if (tcur->getCursorID() == f1id) {

                p1p = p1c;

				p1c.x = tcur->getX();
				p1c.y = tcur->getY();

				consumed = true;

                flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));
			}

			if (tcur->getCursorID() == f2id) {
				
				p2p = p2c;	

				p2c.x = tcur->getX();
				p2c.y = tcur->getY();

				consumed = false;
			}	

            if (consumed == false) {

                p1t = p1c-p1p;
                p2t = p2c-p2p;

                ft.x=std::max(0.0f,std::min(p1t.x,p2t.x)) + std::min(0.0f,std::max(p1t.x,p2t.x));
                ft.y=std::max(0.0f,std::min(p1t.y,p2t.y)) + std::min(0.0f,std::max(p1t.y,p1t.y));

                referenceAngle = atan2((p2p.y - p1p.y) ,(p2p.x - p1p.x));
                newAngle = atan2((p2c.y - p1c.y),(p2c.x - p1c.x));

                cursor.rotate(glm::rotate((newAngle-referenceAngle)*(-50),vec3(0,0,1)));

                cursor.rotate(glm::rotate(ft.x*150,vec3(0,1,0)));
                cursor.rotate(glm::rotate(ft.y*150,vec3(1,0,0)));

                flicker.addRotate(newAngle-referenceAngle);

                consumed = true;
            }

			break;
		}
	}
	if (verbose)
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
		<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
	
}

void Engine::removeTuioCursor(TuioCursor *tcur) {

    switch (appState) {
	case translate:
        flicker.endFlick(glm::mat3(orientation));
		break;
	case rotate:
		switch (rotTechnique) {
        case screenSpace:
            flicker.endRotationFlick();
            break;
		case pinch:
			rotTechnique = screenSpace;
            //std::cout << "screenSpace" << std::endl;
            flicker.endPinchFlick();
			break;
		}
	}

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void Engine::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

void Engine::checkUDP() {
    glm::mat4 newLocationMatrix;
    glm::vec3 newLocationVector;
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

            break;
		case keimote::MAG:
			magnetometerX.update(tempEvent.x());
			magnetometerY.update(tempEvent.y());
			magnetometerZ.update(tempEvent.z());

			ma.x = magnetometerX.get_result();
			ma.y = magnetometerY.get_result();
			ma.z = magnetometerZ.get_result();

			break;
        case keimote::BUTTON:
			switch (tempEvent.buttontype()) {
			case 1:
				calibrate = true;
				break;
			case 2:
                if (appState == direct) {
                    newLocationVector = mat3(orientation)*vec3(0,0,-1);
                    newLocationMatrix = glm::translate(mat4(),newLocationVector);
                    cursor.modelMatrix = newLocationMatrix*cursor.modelMatrix;
                    break;
                }
                if (tempEvent.state() == true && appState != translate) {
                    appState = translate;
                }
                if (tempEvent.state() == false && appState == translate) {
                    appState = rotate;
                    rotTechnique = screenSpace;
                    printf("translate --> rotate");
                }

                break;
            case 3:
                if (appState == direct) {
                    newLocationVector = mat3(orientation)*vec3(0,0,1);
                    newLocationMatrix = glm::translate(mat4(),newLocationVector);
                    cursor.modelMatrix = newLocationMatrix*cursor.modelMatrix;
                    break;
                }
                if (tempEvent.state() == true && appState != direct) {
                    appState = direct;
                }
                if (tempEvent.state() == false && appState == direct) {
                    appState = translate;
                }
            default:
				calibrate = true;
				break;
			}

		};
        //apply matrix to plane
        if (computeRotationMatrix()) {
			plane.modelMatrix[0][0] = orientation[0][0]; plane.modelMatrix[0][1] = orientation[0][1]; plane.modelMatrix[0][2] = orientation[0][2]; 
			plane.modelMatrix[1][0] = orientation[1][0]; plane.modelMatrix[1][1] = orientation[1][1]; plane.modelMatrix[1][2] = orientation[1][2]; 
			plane.modelMatrix[2][0] = orientation[2][0]; plane.modelMatrix[2][1] = orientation[2][1]; plane.modelMatrix[2][2] = orientation[2][2]; 
		}
	}
}

void Engine::checkKeyboard() {
	#define FACTOR 0.5f

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
		perspective +=1.0;
		projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 
		std::cout << "Perspective : " << perspective << '\n';
		
	}
	if (glfwGetKey(GLFW_KEY_SPACE)) {
        cursor.modelMatrix = glm::translate(0,0,-15);
        plane.modelMatrix = cursor.modelMatrix;
        flicker.stopFlick();
		viewMatrix = mat4();
	}
	if (glfwGetKey(GLFW_KEY_END)) {
		perspective -=1.0;
		projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); 
        //log("Perspective : " +perspective);
	}
	
	if (glfwGetKey('1') == GLFW_PRESS) {
        appState = direct;
        log("Direct");
	}

    if (glfwGetKey('2') == GLFW_PRESS) {
        appState = translate;
        log("PlaneCasting");
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
    /*if (cursor.findSphereIntersection(rayOrigin,rayDirection,tempPoint,tempFloat,tempNormal)) {
		previousVector = glm::normalize(glm::vec3(cursor.modelMatrix[3])-tempPoint);
		return true;
	}
    else return false;*/
    return false;

}

void Engine::Drag(const vec3& rayDirection, const vec3& rayOrigin, glm::mat4 viewMatrix) {
    /*glm::vec3 currentVector;
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
    */
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



    //flatShader.use();
    //flatShader["baseColor"] = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    //flatShader["mvp"] = projectionMatrix*pointLight.viewMatrix*cursor.modelMatrix;
    //cursor.bind();
    //CALL_GL(glDrawArrays(GL_TRIANGLES,0,cursor.vertices.size()));

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
        //inputStarted = true;
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

            btTransform objTrans;
            objTrans.setFromOpenGLMatrix(glm::value_ptr(cursor.modelMatrix));
            coCursor->setWorldTransform(objTrans);
        }
	}

}

void Engine::initPhysics()
{
    btTransform temp;

    btSphereShape* csSphere = new btSphereShape(1.0f);
    coCursor = new btCollisionObject();
    temp.setFromOpenGLMatrix(glm::value_ptr(cursor.modelMatrix));
    coCursor->setCollisionShape(csSphere);
    coCursor->setWorldTransform(temp);

    btConvexHullShape* csHeart = new btConvexHullShape();

    for (int i=0;i<heart.vertices.size();++i) {
        csHeart->addPoint(btVector3(heart.vertices[i].x,heart.vertices[i].y,heart.vertices[i].z));
    }
    coHeart = new btCollisionObject();
    temp.setFromOpenGLMatrix(glm::value_ptr(heart.modelMatrix));
    coHeart->setCollisionShape(csHeart);
    coHeart->setWorldTransform(temp);

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btVector3  worldAabbMin(-100,-100,-100);
    btVector3  worldAabbMax(100,100,100);

    btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax);

    collisionWorld = new btCollisionWorld(dispatcher,broadphase,collisionConfiguration);
    //collisionWorld->setDebugDrawer(&debugDrawer);

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    collisionWorld->addCollisionObject(coCursor);
    collisionWorld->addCollisionObject(coHeart);

}

void Engine::physicsCheck()
{
    if (collisionWorld) { collisionWorld->performDiscreteCollisionDetection(); }

    int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
            btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
            btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

            int numContacts = contactManifold->getNumContacts();
            for (int j=0;j<numContacts;j++)
            {

                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getDistance()<0.f)
                {

                    log("we have contact ");

                    const btVector3& ptA = pt.getPositionWorldOnA();
                    const btVector3& ptB = pt.getPositionWorldOnB();
                    const btVector3& normalOnB = pt.m_normalWorldOnB;
                }
            }
        }
}