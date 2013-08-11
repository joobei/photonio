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
    appState(select),
    inputStarted(false),
    mouseMove(false),
    plane(&sr)
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
    doubleClick.start();
    netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));


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

    generateShadowFBO();

    sr.light.position = glm::vec3(0,160,-60);
    sr.light.direction = glm::vec3(0,-1,0);
    sr.light.color = glm::vec4(1,1,1,1);
    sr.light.viewMatrix = glm::lookAt(sr.light.position,glm::vec3(0,0,-60),glm::vec3(0,0,-1));

    //*************************************************************
    //********************  Load Shaders **************************
    //*************************************************************
    sr.flatShader = pho::Shader(shaderpath+"flat");

    noTextureShader = pho::Shader(shaderpath+"notexture");
    noTextureShader.use();
    noTextureShader["view"] = sr.viewMatrix;
    noTextureShader["light_position"] = glm::vec4(pointLight.position,1);
    noTextureShader["light_diffuse"] = pointLight.color;
    noTextureShader["light_specular"] = vec4(1,1,1,1);

    textureShader = pho::Shader(shaderpath+"texture");
    textureShader.use();
    textureShader["view"] = sr.viewMatrix;
    textureShader["light_position"] = glm::vec4(pointLight.position,1);
    textureShader["light_diffuse"] = pointLight.color;
    textureShader["light_specular"] = vec4(1,1,1,1);

    GLuint t1Location = glGetUniformLocation(textureShader.program, "diffuseTexture");
    GLuint t2Location = glGetUniformLocation(textureShader.program, "normalMap");
    GLuint t3Location = glGetUniformLocation(textureShader.program, "shadowMap");

    glUniform1i(t1Location, 0);
    glUniform1i(t2Location, 1);
    glUniform1i(t3Location, 2);

    //shadow map debug
    smdebug = pho::Shader(shaderpath+"texader");
    smdebug.use();
    shadowMapLoc = glGetUniformLocation(smdebug.program, "shadowMap");
    baseImageLoc = glGetUniformLocation(smdebug.program, "texturex");

    glUniform1i(baseImageLoc, 0);
    glUniform1i(shadowMapLoc, 2);

    //*************************************************************
    //********************  Load Assets ***************************
    //*************************************************************
    cursor = pho::Asset("cursor.obj", &noTextureShader,&sr);
    cursor.modelMatrix = glm::translate(glm::mat4(),glm::vec3(0,0,-5));
    selectedAsset = &cursor; //when app starts we control the cursor
    //cursor.receiveShadow = true;

    //floor = pho::Asset("floor.obj", &textureShader,&sr);
    floor = pho::Asset("floor.obj", &smdebug,&sr);
    floor.modelMatrix  = glm::translate(glm::mat4(),glm::vec3(0,-20,-60));
    floor.receiveShadow = true;

    plane.setShader(&flatShader);
    plane.modelMatrix = cursor.modelMatrix;
    plane.setScale(15.0f);
    //plane.receiveShadow = true;

    heart = pho::Asset("bump-heart.obj",&textureShader,&sr);
    heart.modelMatrix = glm::translate(glm::mat4(),glm::vec3(0,0,-45));
    //heart.receiveShadow = true;
    //Create the perspective matrix
    sr.projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f);

    cameraPosition = vec3(0,0,0);
    sr.viewMatrix = glm::lookAt(cameraPosition,vec3(0,0,-1),vec3(0,1,0));

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    //debug floor
    CALL_GL(glGenVertexArrays(1,&floorVao));
    CALL_GL(glBindVertexArray(floorVao));
    float vertices[] = {-10.0f,0.0f,10.0f,
                        0.0f,0.0f,-10.0f,
                        10.0f,0.0f,10.0f,
                       0.0f,0.0f,-10.0f,
                         10.0f,0.0f,10.0f,
                        15.0f,0.0f,10.0f,
                       };

    float uv[] = {0.f,1.f,0.f,0.f,1.f,0.f,1.f,1.f,0.f,1.f,1.f,0.f};

    GLuint buffer;
    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*18,vertices, GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*6, uv, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoordLoc);
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);

    CALL_GL(glBindVertexArray(0));

    CALL_GL(glActiveTexture(GL_TEXTURE0));
    CALL_GL(glGenTextures(1,&floorTexture));
    floorTexture = gli::createTexture2D("assets/grid.dds");

    floorMatrix = glm::translate(glm::mat4(),glm::vec3(0,-12,-25));

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
            selectedAsset->modelMatrix = glm::translate(vec3(0,0,-amount))*selectedAsset->modelMatrix;
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
        pho::locationMatch(selectedAsset->modelMatrix,plane.modelMatrix);  //put cursor in plane's location
        //selectedAsset->modelMatrix = flickTransform*selectedAsset->modelMatrix;
        //plane.modelMatrix = selectedAsset->modelMatrix;
    }

    if (flicker.inPinchFlick()) {
        glm::mat4 flickTransform = flicker.dampenAndGivePinchMatrix();
        selectedAsset->rotate(flickTransform);
    }

    if (flicker.inRotationFlick()){
        glm::vec2 rotation = flicker.dampenAndGiveRotationMatrix();
        selectedAsset->rotate(glm::rotate(rotation.x*3.0f,vec3(0,1,0)));
        selectedAsset->rotate(glm::rotate(rotation.y*3.0f,vec3(1,0,0)));
    }

    //Joystick
    checkSpaceNavigator();

    if (doubleClickPerformed && (appState == translate || appState == rotate)) {

        cursor.modelMatrix[3] = glm::vec4(glm::vec3(selectedAsset->modelMatrix[3])+grabbedVector ,1);
        //grabbedVector = glm::vec3(cursor.modelMatrix[3])-glm::vec3(it->second->modelMatrix[3]); //just for reference
        cursor.modelMatrix[3] = glm::vec4(glm::vec3(selectedAsset->modelMatrix[3])+grabbedVector,1);
        selectedAsset = &cursor;
        pho::locationMatch(plane.modelMatrix,cursor.modelMatrix);
        appState = select;
        doubleClickPerformed = false;
    }
}

void Engine::render() {
    shadowMapRender();

    CALL_GL(glClearColor(0.0f,0.0f,0.0f,0.0f));
    CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    //if (!inputStarted) { heart.rotate(glm::rotate(0.1f,glm::vec3(0,1,0))); }
    floor.draw();
    heart.draw();
    if (appState == select) {
        cursor.draw();
    }
    if (technique == planeCasting && appState != rotate) {
        plane.draw();
    }

    glfwSwapBuffers();

    checkPhysics();
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

    boost::timer::cpu_times const elapsed_times(doubleClick.elapsed());
    double difference = elapsed_times.wall-previousTime.wall;
    //cout.precision(15);
    //std::cout << "Elapsed :" << difference << std::endl;
    //check for double click

    if (difference < 150000000) {
        doubleClickPerformed = true;
    }

    previousTime = elapsed_times;


    if ((button == GLFW_MOUSE_BUTTON_1) && (state == GLFW_PRESS))
    {
        float norm_x = 1.0*cur_mx/WINDOW_SIZE_X*2 - 1.0;
        float norm_y = -(1.0*cur_my/WINDOW_SIZE_Y*2 - 1.0);

        glm::vec4 mouse_clip = glm::vec4((float)cur_mx * 2 / float(WINDOW_SIZE_X) - 1, 1 - float(cur_my) * 2 / float(WINDOW_SIZE_Y),0,1);

        glm::vec4 mouse_world = glm::inverse(sr.viewMatrix) * glm::inverse(sr.projectionMatrix) * mouse_clip;

        rayOrigin = glm::vec3(sr.viewMatrix[3]);
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

    glm::vec4 mouse_world = glm::inverse(sr.viewMatrix) * glm::inverse(sr.projectionMatrix) * mouse_clip;

    rayOrigin = glm::vec3(sr.viewMatrix[3]);
    rayDirection = glm::normalize(glm::vec3(mouse_world)-rayOrigin);

    if (appState == rotate) {
        Drag(rayDirection,rayOrigin,sr.viewMatrix);
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
            selectedAsset->modelMatrix = glm::translate(glm::vec3(difference,0))*selectedAsset->modelMatrix;
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
    std::list<TUIO::TuioCursor*> cursorList;
    cursorList = tuioClient->getTuioCursors();
    short numberOfCursors = cursorList.size();
    //std::cout << "Added cursor, Current NoOfCursors " << numberOfCursors << std::endl;

    //notify flick manager of a new gesture starting
    if (numberOfCursors == 1) {
        flicker.newFlick();
        flicker.stopPinchFlick();
    }

    boost::timer::cpu_times const elapsed_times(doubleClick.elapsed());
    double difference = elapsed_times.wall-previousTime.wall;
    //cout.precision(15);
    //std::cout << "Elapsed :" << difference << std::endl;
    //check for double click
    if (numberOfCursors == 1 ) {
        if (difference < 150000000) {
        doubleClickPerformed = true;
        }
    }
    previousTime = elapsed_times;


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
        pho::locationMatch(selectedAsset->modelMatrix,plane.modelMatrix);  //put cursor in plane's location

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
                selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

                p1c.x = tcur->getX();
                p1c.y = tcur->getY();

                flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));
            }

            if (tcur->getCursorID() == f2id) {

                p2p = p2c;
                selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

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

                selectedAsset->rotate(glm::rotate((newAngle-referenceAngle)*(-50),vec3(0,0,1)));

                selectedAsset->rotate(glm::rotate(ft.x*150,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(ft.y*150,vec3(1,0,0)));

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
    case select:
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
                    selectedAsset->modelMatrix = newLocationMatrix*selectedAsset->modelMatrix;
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
                    selectedAsset->modelMatrix = newLocationMatrix*selectedAsset->modelMatrix;
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
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(0,0,-FACTOR));
    }

    if (glfwGetKey(GLFW_KEY_UP)) {
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(0,0,FACTOR));
    }
    if (glfwGetKey(GLFW_KEY_LEFT)) {
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(FACTOR,0,0));
    }
    if (glfwGetKey(GLFW_KEY_RIGHT)) {
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(-FACTOR,0,0));
    }
    if (glfwGetKey(GLFW_KEY_PAGEUP)) {
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(0,-FACTOR,0));
    }
    if (glfwGetKey(GLFW_KEY_PAGEDOWN)) {
        sr.viewMatrix = glm::translate(sr.viewMatrix, vec3(0,FACTOR,0));
    }
    if (glfwGetKey(GLFW_KEY_HOME)) {
        perspective +=1.0;
        sr.projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f);
        std::cout << "Perspective : " << perspective << '\n';

    }
    if (glfwGetKey(GLFW_KEY_SPACE)) {
        selectedAsset->modelMatrix = glm::translate(0,0,-15);
        plane.modelMatrix = selectedAsset->modelMatrix;
        flicker.stopFlick();
        sr.viewMatrix = mat4();
    }
    if (glfwGetKey(GLFW_KEY_END)) {
        perspective -=1.0;
        sr.projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f);
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

    CALL_GL(glGenTextures(1, &(sr.shadowTexture)));
    CALL_GL(glActiveTexture(GL_TEXTURE0));
    CALL_GL(glBindTexture(GL_TEXTURE_2D, sr.shadowTexture));
    CALL_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0));
    CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS));
    CALL_GL(glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,shadowMapWidth,shadowMapHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL));
    CALL_GL(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE));
    CALL_GL(glBindTexture(GL_TEXTURE_2D, 0)); //unbind the texture

    CALL_GL(glGenFramebuffers(1, &shadowFBO));
    CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO));
    CALL_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sr.shadowTexture, 0));
    CALL_GL(glDrawBuffer(GL_NONE));
    CALL_GL(glReadBuffer(GL_NONE));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    { printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER)); }

    CALL_GL(glClearDepth(1.0f); glEnable(GL_DEPTH_TEST));
    // Needed when rendering the shadow map. This will avoid artifacts.
    CALL_GL(glPolygonOffset(1.0f, 0.0f); glBindFramebuffer(GL_FRAMEBUFFER, 0));
    //to convert the texture coordinates to -1 ~ 1
    GLfloat biasMatrixf[] = {
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f };

    sr.biasMatrix = glm::make_mat4(biasMatrixf);
}

void Engine::shadowMapRender() {
    int shadowMapWidth = WINDOW_SIZE_X * (int)SHADOW_MAP_RATIO;
    int shadowMapHeight =  WINDOW_SIZE_Y * (int)SHADOW_MAP_RATIO;

    // Rendering into the shadow texture.
    CALL_GL(glActiveTexture(GL_TEXTURE0));
    CALL_GL(glBindTexture(GL_TEXTURE_2D, sr.shadowTexture));
    // Bind the framebuffer.
    CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO));
    //Clear it
    CALL_GL(glClear(GL_DEPTH_BUFFER_BIT));
    CALL_GL(glViewport(0, 0, shadowMapWidth, shadowMapHeight));
    CALL_GL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

    if (appState == select) cursor.drawFromLight();
    heart.drawFromLight();

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

            sr.viewMatrix = glm::translate(vec3(-1*position[0]*TRSCALE,0,0))*sr.viewMatrix;
            sr.viewMatrix = glm::translate(vec3(0,position[2]*TRSCALE,0))*sr.viewMatrix;
            sr.viewMatrix = glm::translate(vec3(0,0,position[1]*TRSCALE))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*-1*position[5],glm::vec3(0,1,0))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*-1*position[4],glm::vec3(0,0,1))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*position[3],glm::vec3(1,0,0))*sr.viewMatrix;
        }
        else {
            selectedAsset->modelMatrix = glm::translate(vec3(position[0]*TRSCALE,0,0))*selectedAsset->modelMatrix;
            selectedAsset->modelMatrix = glm::translate(vec3(0,-1*position[2]*TRSCALE,0))*selectedAsset->modelMatrix;
            selectedAsset->modelMatrix = glm::translate(vec3(0,0,-1*position[1]*TRSCALE))*selectedAsset->modelMatrix;
            selectedAsset->rotate(glm::rotate(RTSCALE*position[5],glm::vec3(0,1,0)));
            selectedAsset->rotate(glm::rotate(RTSCALE*position[4],glm::vec3(0,0,1)));
            selectedAsset->rotate(glm::rotate(RTSCALE*-1*position[3],glm::vec3(1,0,0)));

            btTransform objTrans;
            objTrans.setFromOpenGLMatrix(glm::value_ptr(selectedAsset->modelMatrix));
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

    selectMap.insert(std::make_pair(coCursor,&cursor));

    btConvexHullShape* csHeart = new btConvexHullShape();

    for (int i=0;i<heart.vertices.size();++i) {
        csHeart->addPoint(btVector3(heart.vertices[i].x,heart.vertices[i].y,heart.vertices[i].z));
    }
    coHeart = new btCollisionObject();
    temp.setFromOpenGLMatrix(glm::value_ptr(heart.modelMatrix));
    coHeart->setCollisionShape(csHeart);
    coHeart->setWorldTransform(temp);

    selectMap.insert(std::make_pair(coHeart,&heart));

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btVector3  worldAabbMin(-100,-100,-100);
    btVector3  worldAabbMax(100,100,100);

    btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax);

    collisionWorld = new btCollisionWorld(dispatcher,broadphase,collisionConfiguration);
    //collisionWorld->setDebugDrawer(&debugDrawer);

    //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    collisionWorld->addCollisionObject(coCursor);
    collisionWorld->addCollisionObject(coHeart);

}

void Engine::checkPhysics()
{
    btTransform temp;

    temp.setFromOpenGLMatrix(glm::value_ptr(heart.modelMatrix));
    coHeart->setWorldTransform(temp);

    temp.setFromOpenGLMatrix(glm::value_ptr(cursor.modelMatrix));
    coCursor->setWorldTransform(temp);

    if (appState == select) {

        if (collisionWorld) { collisionWorld->performDiscreteCollisionDetection(); }

        std::map<btCollisionObject*,pho::Asset*>::iterator it;
        int numManifolds = collisionWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  collisionWorld->getDispatcher()->getManifoldByIndexInternal(i);
            btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
            btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

            it = selectMap.find(obB);
            if ( it != selectMap.end()) {
                it->second->beingIntersected = true;
                if (doubleClickPerformed) {
                    switch (appState) {
                    case select:
                        selectedAsset = it->second;
                        grabbedVector = glm::vec3(cursor.modelMatrix[3])-glm::vec3(it->second->modelMatrix[3]);
                        plane.modelMatrix = it->second->modelMatrix;
                        appState = translate;
                        doubleClickPerformed = false;
                        break;
                    }

                }
            }


            /*int numContacts = contactManifold->getNumContacts();
            for (int j=0;j<numContacts;j++)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getDistance()<0.f)
                {
                    const btVector3& ptA = pt.getPositionWorldOnA();
                    const btVector3& ptB = pt.getPositionWorldOnB();
                    const btVector3& normalOnB = pt.m_normalWorldOnB;
                }
            }*/
        }

    }
}
