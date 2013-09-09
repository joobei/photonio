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
#include <glm/gtc/type_ptr.hpp>

using namespace std;

#define USER "Nicholas"

//factor for translation in planeCasting
#define TFACTORA 10
#define FACTORB 2.0f

Engine::Engine():
    calibrate(false),
    eventQueue(),
    udpwork(ioservice),
    _udpserver(ioservice,&eventQueue,&ioMutex),
    _serialserver(serialioservice,115200,"/dev/ttyUSB0",&eventQueue,&ioMutex),
    appState(translate),
    selectionTechnique(indieSelectRelative),
    plane(&sr),
    rotTechnique(screenSpace),
    technique(planeCasting),
    pyramidCursor(&sr),
    target(&sr),
    ray(&sr)
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
    keyboardTimer.start();

    netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));

    //Polhemus
    serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));

    prevMouseWheel = 0;
    gyroData = false;
    objectHit=false;
    sphereHit=false;

    axisChange[0][0] = 1; axisChange[0][1] =  0;  axisChange[0][2] =  0;
    axisChange[1][0] = 0; axisChange[1][1] =  0;  axisChange[1][2] =  1;
    axisChange[2][0] = 0; axisChange[2][1] =  -1;  axisChange[2][2] = 0;

    tf = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S");

    perspective = 80.0f;

    consumed = false;

    pointerOpacity = 1.0f;

    experiment.user = USER;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &sr.fLargest);

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
    initPhysics();

    //sr.light.position = vec3(0,160,-60);
    sr.light.position = vec3(0,30,0);

    sr.light.direction = vec3(0,-1,0);
    sr.light.color = vec4(1,1,1,1);
    sr.light.viewMatrix = glm::lookAt(sr.light.position,vec3(0,0,0),vec3(0,0,-5));

    //*************************************************************
    //********************  Load Shaders **************************
    //*************************************************************
    sr.flatShader = pho::Shader(shaderpath+"flat");

    sr.noTextureShader = pho::Shader(shaderpath+"notexture");
    sr.noTextureShader.use();
    sr.noTextureShader["view"] = sr.viewMatrix;
    sr.noTextureShader["light_position"] = vec4(pointLight.position,1);
    sr.noTextureShader["light_diffuse"] = pointLight.color;
    sr.noTextureShader["light_specular"] = vec4(1,1,1,1);

    normalMap = pho::Shader(shaderpath+"texture");
    normalMap.use();
    normalMap["view"] = sr.viewMatrix;
    normalMap["light_position"] = vec4(pointLight.position,1);
    normalMap["light_diffuse"] = pointLight.color;
    normalMap["light_specular"] = vec4(1,1,1,1);

    sr.colorShader = pho::Shader(shaderpath+"color");
    sr.lineShader = pho::Shader(shaderpath+"lines");
    GLuint t1Location = glGetUniformLocation(normalMap.program, "diffuseTexture");
    GLuint t2Location = glGetUniformLocation(normalMap.program, "normalMap");
    GLuint t3Location = glGetUniformLocation(normalMap.program, "shadowMap");

    glUniform1i(t1Location, 0);
    glUniform1i(t2Location, 1);
    glUniform1i(t3Location, 2);

    //shadow map debug
    singleTexture = pho::Shader(shaderpath+"texader");
    singleTexture.use();
    shadowMapLoc = glGetUniformLocation(singleTexture.program, "shadowMap");
    baseImageLoc = glGetUniformLocation(singleTexture.program, "texturex");

    glUniform1i(baseImageLoc, 0);
    glUniform1i(shadowMapLoc, 2);


    sr.lineShader = pho::Shader(shaderpath+"lines");
    sr.flatLitShader = pho::Shader(shaderpath+"flatLit");

    //*************************************************************
    //********************  Load Assets ***************************
    //*************************************************************
    cursor = pho::Asset("cursor.obj", &sr.noTextureShader,&sr);
    cursor.modelMatrix = glm::translate(mat4(),vec3(0,0,-5));
    selectedAsset = &pyramidCursor; //when app starts we control the cursor
    //cursor.receiveShadow = true;

    target.setAlpha(0.6);
    target.color = glm::vec3(0.6,0.6,0.6);

    s0 = pho::Asset("cursor.obj", &sr.flatLitShader,&sr);
    s0.setScale(0.1f);
    s1 = pho::Asset("cursor.obj", &sr.flatLitShader,&sr);
    s1.setScale(0.1f);
    s2 = pho::Asset("cursor.obj", &sr.flatLitShader,&sr);
    s2.setScale(0.1f);
    s3 = pho::Asset("cursor.obj", &sr.flatLitShader,&sr);
    s3.setScale(0.1f);

    floor = pho::Asset("floor.obj", &singleTexture,&sr);
    floor.modelMatrix  = glm::translate(mat4(),vec3(0,-6,0));
    floor.receiveShadow = true;

    plane.setShader(&sr.flatShader);
    plane.modelMatrix = pyramidCursor.modelMatrix;
    plane.setScale(3.0f);
    //plane.receiveShadow = true;

    pyramidCursor.modelMatrix = glm::translate(mat4(),vec3(0,0,0));

    target.setPosition(vec3(6,0,2));

    //load texture
    ray.texture = gli::createTexture2D(assetpath+"grad2.dds");
    ray.setAlpha(1.0f);

    vec3 disc = vec3(0.0,0.0,0.0);
    GLuint buffer;

    glGenVertexArrays(1,&pointVao);
    glBindVertexArray(pointVao);

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3, glm::value_ptr(disc), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

    //Create the perspective matrix
    sr.projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f);

    cameraPosition = vec3(0,0,10);
    sr.viewMatrix = glm::lookAt(cameraPosition,vec3(0,0,-1),vec3(0,1,0));

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);


    InverseProjectionMatrix = glm::inverse(sr.projectionMatrix);
    InverseViewMatrix = glm::inverse(sr.viewMatrix);

    CALL_GL(glEnable(GL_PROGRAM_POINT_SIZE_EXT));

    //experiment manager
    experiment.setCursor(&pyramidCursor);
    experiment.setTarget(&target);
    experiment.setUser(USER);
    experiment.wandPosition = &polhemusMatrix;
}

//checks event queue for events
//and consumes them all
void Engine::checkEvents() {
    //checkMove();
    unsigned char buttons[19];

    checkKeyboard();
    if ((technique == rayCasting) && (appState == select)) {
        checkPolhemus(ray.modelMatrix);
        glfwGetJoystickButtons(moveController,buttons,19);
    }
    if ((technique == rayCasting) && (appState == direct)) {
        checkPolhemus(polhemusMatrix);
        glfwGetJoystickButtons(moveController,buttons,19);
        for (auto i=0;i<19;++i) {
            experiment.buttons[i] = buttons[i];
        }
    }

    if (technique == mouse) {
        float wheel = glfwGetMouseWheel();
        if (wheel != prevMouseWheel) {
            float amount = (wheel - prevMouseWheel)/10;
            selectedAsset->modelMatrix = glm::translate(vec3(0,0,-amount))*selectedAsset->modelMatrix;
            prevMouseWheel = wheel;
        }
    }

    if (technique == planeCasting) {
        checkUDP();
    }

    if (flicker.inFlick(translation)) {
        mat4 flickTransform = flicker.dampenAndGiveMatrix(mat3(plane.modelMatrix));
        plane.modelMatrix = flickTransform*plane.modelMatrix;  //translate plane
        pho::locationMatch(selectedAsset->modelMatrix,plane.modelMatrix);  //put cursor in plane's location
        //selectedAsset->modelMatrix = flickTransform*selectedAsset->modelMatrix;
        //plane.modelMatrix = selectedAsset->modelMatrix;
    }

    if (flicker.inFlick(pinchy)) {
        mat4 flickTransform = flicker.dampenAndGivePinchMatrix();
        selectedAsset->rotate(flickTransform);
    }

    if (flicker.inFlick(rotation)){
        glm::vec2 rotation = flicker.dampenAndGiveRotationMatrix();
        selectedAsset->rotate(glm::rotate(rotation.x*3.0f,vec3(0,1,0)));
        selectedAsset->rotate(glm::rotate(rotation.y*3.0f,vec3(1,0,0)));
    }

    //Joystick
    checkSpaceNavigator();
    //checkMove();

    if (appState == direct) {

        //rotation
        if ((buttons[12] == GLFW_PRESS) && (directRotationStarted == false))
        {
                objectInitMatrix = selectedAsset->modelMatrix;
                receiverInitMatrix = polhemusMatrix;
                directRotationStarted = true;
        }

        if(directRotationStarted) {
            rotationMatch(selectedAsset->modelMatrix,polhemusMatrix*glm::inverse(receiverInitMatrix)*objectInitMatrix);
        }

        if ((buttons[11] == GLFW_PRESS) && (directTranslationStarted == false))
        {

            initPosition = rayOrigin;
            directTranslationStarted = true;
        }

        if (directTranslationStarted) {

                glm::vec3 translationVector = rayOrigin-initPosition;
                if (glm::length(translationVector) < 5) {
                    mat4 tm = mat4();
                    rotationMatch(tm,selectedAsset->modelMatrix);
                    vec3 pos = vec3(selectedAsset->modelMatrix[3]);
                    selectedAsset->modelMatrix = glm::mat4();
                    selectedAsset->setPosition(pos);
                    selectedAsset->modelMatrix = glm::translate(selectedAsset->modelMatrix,translationVector);
                    rotationMatch(selectedAsset->modelMatrix,tm);


                    //selectedAsset->setPosition(glm::vec3(temp));
                }
                initPosition = rayOrigin;
        }

        if (buttons[12] == GLFW_RELEASE) {
           directRotationStarted = false;
        }
        if (buttons[11] == GLFW_RELEASE) {
           directTranslationStarted = false;
        }
        /*//translation
        //vec3 V_hand = (-position) - (-prevPosition);
        vec3 V_hand = (ray.getPosition()) - (receiverPrevPosition);

        float SC = 0.007;

        float V_hand_d = 1.2 < glm::length(V_hand) / SC ? 1.2 : glm::length(V_hand) / SC; //speed scaling
        selectedAsset->modelMatrix[3][0] = 50*V_hand_d*V_hand[0]+objectprevModelMatrix[3][0];
        selectedAsset->modelMatrix[3][1] = 50*V_hand_d*V_hand[1]+objectprevModelMatrix[3][1];
        selectedAsset->modelMatrix[3][2] = 50*V_hand_d*V_hand[2]+objectprevModelMatrix[3][2];

        receiverPrevPosition = ray.getPosition();
        objectprevModelMatrix = selectedAsset->modelMatrix;*/
        //LOG(sqrtf(pow(V_hand[0],2)+pow(V_hand[1],2)+pow(V_hand[2],2)));

    }


    if ((appState == select)) {
        if (selectionTechnique == indieSelectRelative) {
            if (rayTest(touchPoint.x,touchPoint.y,intersectedAsset))
            {
                intersectedAsset->beingIntersected = true;
            }
        }

        if (technique == rayCasting) {

            if (rayTestWorld(rayOrigin,rayDirection,intersectedAsset))
            {
                intersectedAsset->beingIntersected = true;
                //for shortening ray. was weird so I cut it.
                /*vec3 length = intersectionPoint-ray.getPosition();
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vbo));
                CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,3*sizeof(float),3*sizeof(float),glm::value_ptr(length)));
            }
            else
            {
                //no need to do this every frame
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vbo));
                CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,3*sizeof(float),3*sizeof(float),glm::value_ptr(vec3(0,0,-1000))));
            }*/
            }



            if ((buttons[11] == GLFW_PRESS ) && (rayTestWorld(rayOrigin,rayDirection,intersectedAsset)))
            {
                appState = direct;
                selectedAsset = intersectedAsset;              

                directTranslationStarted = true;
                initPosition = rayOrigin;
            }
        }
    }




    checkPhysics();

    /*if(switchOnNextFrame) {
    pho::locationMatch(plane.modelMatrix,cursor.modelMatrix);
    //Put 2d cursor where object was dropped:
    //1st. calculate clip space coordinates
    vec4 newtp = sr.projectionMatrix*sr.viewMatrix*selectedAsset->modelMatrix*vec4(0,0,0,1);
    //2nd. convert it in normalized device coordinates by dividing with w
    newtp/=newtp.w;
    touchPoint.x = newtp.x;
    touchPoint.y = newtp.y;
    switchOnNextFrame = false;
    }*/

    if(touchPoint.x < -1.0f) { touchPoint.x = -1.0; }
    if(touchPoint.x > 1.0f) { touchPoint.x = 1; }
    if(touchPoint.y < -1.0f) { touchPoint.y = -1.0; }
    if(touchPoint.y > 1.0f) { touchPoint.y = 1; }

    std::list<TuioCursor*> list = tuioClient->getTuioCursors();

    if (list.size() == 0)
    {
     experiment.finger1.exists = false;
     experiment.finger1.x = 0;
     experiment.finger1.y = 0;

     experiment.finger2.exists = false;
     experiment.finger2.x = 0;
     experiment.finger2.y = 0;
    }

    if (list.size() == 1)
    {
     experiment.finger1.exists = true;
     experiment.finger1.x = list.front()->getPosition().getX();
     experiment.finger1.y = list.front()->getPosition().getY();
     experiment.finger1.id = list.front()->getCursorID();

     experiment.finger2.exists = false;
     experiment.finger2.x = 0;
     experiment.finger2.y = 0;
    }

    if (list.size() == 2)
    {
     experiment.finger1.exists = true;
     experiment.finger1.x = list.front()->getPosition().getX();
     experiment.finger1.y = list.front()->getPosition().getY();
     experiment.finger1.id = list.front()->getCursorID();


     experiment.finger2.exists = true;
     experiment.finger2.x = list.back()->getPosition().getX();
     experiment.finger2.y = list.back()->getPosition().getY();
     experiment.finger2.id = list.back()->getCursorID();
    }

    experiment.log();
}

void Engine::render() {

    shadowMapRender();

    CALL_GL(glClearColor(1.0f,1.0f,1.0f,0.0f));
    CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    floor.draw();

    pyramidCursor.draw();

    //match spheres to vertex positions before drawing them
    s0.rotateAboutAsset(pyramidCursor.modelMatrix,pyramidCursor.v0);
    s0.opacity = 1.0;
    s0.drawPlain(red);


    s1.rotateAboutAsset(pyramidCursor.modelMatrix,pyramidCursor.v1);
    s1.opacity = 1.0;
    s1.drawPlain(green);

    s2.rotateAboutAsset(pyramidCursor.modelMatrix,pyramidCursor.v2);
    s2.opacity = 1.0;
    s2.drawPlain(blue);

    s3.rotateAboutAsset(pyramidCursor.modelMatrix,pyramidCursor.v3);
    s3.opacity = 1.0;
    s3.drawPlain(yellow);

    target.draw();

    s0.rotateAboutAsset(target.modelMatrix,pyramidCursor.v0);
    s0.opacity = 0.6;
    s0.drawPlain(red);

    s1.rotateAboutAsset(target.modelMatrix,pyramidCursor.v1);
    s1.opacity = 0.6;
    s1.drawPlain(green);

    s2.rotateAboutAsset(target.modelMatrix,pyramidCursor.v2);
    s2.opacity = 0.6;
    s2.drawPlain(blue);

    s3.rotateAboutAsset(target.modelMatrix,pyramidCursor.v3);
    s3.opacity = 0.6;
    s3.drawPlain(yellow);


    if (appState == select) {
        if (selectionTechnique == raySelect)
        {
            ray.draw();
        }
        else
        {
            sr.flatShader.use();

            //sr.flatShader["mvp"] = sr.projectionMatrix*sr.viewMatrix*mat4();
            sr.flatShader["mvp"] = glm::translate(mat4(),vec3(touchPoint.x,touchPoint.y,-1));
            CALL_GL(glBindVertexArray(pointVao));
            CALL_GL(glPointSize(20));
            sr.flatShader["color"] = vec4(0,0,0,pointerOpacity);
            CALL_GL(glDrawArrays(GL_POINTS,0,1));

            CALL_GL(glDisable(GL_DEPTH_TEST));
            sr.flatShader["color"] = vec4(1,1,1,pointerOpacity);
            CALL_GL(glPointSize(10));
            CALL_GL(glDrawArrays(GL_POINTS,0,1));
            CALL_GL(glEnable(GL_DEPTH_TEST));

            //pointerOpacity-=0.1;
        }
    }

    if ((technique == planeCasting) && (appState == translate)) {
        plane.draw();
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
    serialioservice.stop();
    experiment.closeFiles();
    //psmove_disconnect(move);
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

    float x,y;
    x = tcur->getX();
    y = tcur->getY();

    //TUIO variables
    std::list<TUIO::TuioCursor*> cursorList;
    cursorList = tuioClient->getTuioCursors();
    short numberOfCursors = cursorList.size();
    //std::cout << "Added cursor, Current NoOfCursors " << numberOfCursors << std::endl;

    //notify flick manager of a new gesture starting
    if ((numberOfCursors == 1) && ( appState == translate || appState == select))
    {
        flicker.newFlick(flickState::translation);
        flicker.stopFlick(flickState::translation); // remove this for flick continue "feature"
        flicker.stopFlick(flickState::pinchy);
        p1c.x = x;
        p1c.y = y;
        f1id = tcur->getCursorID();

        p1p = p1c ;  //when first putting finger down there must be
    }

    if ((numberOfCursors == 2) && (appState == translate)) {
        p2c.x = x;
        p2c.y = y;
        f2id = tcur->getCursorID();
        lastFingerDistance = glm::distance(p2c,p1c);
        p2p = p2c ;  //when first putting finger down there must be
    }

    if ((numberOfCursors == 1) && ( appState == rotate))
    {
        flicker.stopFlick(flickState::rotation);
        flicker.newFlick(flickState::rotation);
    }

    boost::timer::cpu_times const elapsed_times(doubleClick.elapsed());
    double difference = elapsed_times.wall-previousTime.wall;

    /*if (numberOfCursors == 1 ) {
        if (difference < 150000000)
        {  //if doubleClick

            if((appState == select) && rayTest(touchPoint.x,touchPoint.y,intersectedAsset))
            {
                selectedAsset = intersectedAsset;
                appState = translate;
            }

            if ((appState == translate) || (appState == rotate)) {

                //Put 2d cursor where object was dropped:
                //1st. calculate clip space coordinates
                vec4 newtp = sr.projectionMatrix*sr.viewMatrix*pyramidCursor.modelMatrix*vec4(0,0,0,1);
                //2nd. convert it in normalized device coordinates by dividing with w
                newtp/=newtp.w;
                touchPoint.x = newtp.x;
                touchPoint.y = newtp.y;

            }
            appState = select;
        }

    }

    previousTime = elapsed_times;*/


    switch (appState) {
    case select:
        if ((numberOfCursors == 2) && rayTest(touchPoint.x,touchPoint.y,intersectedAsset)) {
            appState = translate;
            selectedAsset = intersectedAsset;
            pho::locationMatch(plane.modelMatrix,intersectedAsset->modelMatrix);

            p2c.x = x;
            p2c.y = y;
            f2id = tcur->getCursorID();
            lastFingerDistance = glm::distance(p2c,p1c);
            p2p = p2c ;  //when first putting finger down there must be
            break;
        }
        if ((selectionTechnique == indieSelectHybrid)) {
            touchPoint.x = 2*x-1;
            touchPoint.y = 2*(1-y)-1;
            break;
        }
        break;
    case translate:
        break;
    case rotate:
        if (numberOfCursors == 0) {
            flicker.stopFlick(rotation);
        }
        if (numberOfCursors == 1) {
            p1c.x = x;
            p1c.y = y;
            f1id = tcur->getCursorID();

            p1p = p1c ;  //when first putting finger down there must be
            // a previous spot otherwise when the other finger moves it goes crazy
            p1f = p1c; //save 1st touched point
        }
        if (numberOfCursors == 2) {



            if ((p1f.x > 0.8) && (p1f.y < 0.2))
            {

                rotTechnique = locky;
                //std::cout << "Lock Y axis" << std::endl;
            }
            else if ((p1f.x < 0.2) && (p1f.y > 0.8))
            {

                rotTechnique = lockx;
                //std::cout << "Lock X axis" << std::endl;
            }
            else if ((p1f.x < 0.6) && (p1f.x > 0.4) && (p1f.y > 0.4) && (p1f.y < 0.6) && (x < 0.6) && (x > 0.4) && (y > 0.4) && (y < 0.6))
            {

                rotTechnique = lockz;
               // std::cout << "Lock Z axis" << std::endl;
            }
            else
            {

                rotTechnique = pinch;
                std::cout << "pinch rotate" << '\n';


                p2p = p2c;   //when first putting finger down there must be
                // a previous spot otherwise when the other finger moves it goes crazy

                consumed = true;
            }
            flicker.stopFlick(pinchy);
            p2c.x = x;
            p2c.y = y;
            f2id = tcur->getCursorID();

            std::cout << "Distance : " << glm::distance(p1c,p2c) << std::endl;
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

    flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));

    vec2 ft;
    float newAngle;
    short numberOfCursors = tuioClient->getTuioCursors().size();

    switch (appState) {
    case select:
        touchPoint.x += tcur->getXSpeed()/30;
        touchPoint.y += -1*tcur->getYSpeed()/30;
        break;
    case translate:
        //********************* TRANSLATE ****************************
        if (numberOfCursors == 2)
        {
            if (tcur->getCursorID() == f1id) {
                p1c.x = x;
                p1c.y = y;
            }

            if (tcur->getCursorID() == f2id) {
                p2c.x = x;
                p2c.y = y;
            }
            float currentFingerDistance = glm::distance(p2c,p1c);
            float distanceTravelled = lastFingerDistance-currentFingerDistance;

            tempMat = mat3(orientation);  //get the rotation part from the plane's matrix
            newLocationVector = tempMat*vec3(0,distanceTravelled,0);

            if (distanceTravelled < 0.5)
            {
                newLocationMatrix = glm::translate(mat4(),newLocationVector);   //Calculate new location by translating object by motion vector
                plane.modelMatrix = newLocationMatrix*plane.modelMatrix;
                pho::locationMatch(selectedAsset->modelMatrix,plane.modelMatrix);
            }

            lastFingerDistance = currentFingerDistance;
        }
        else
        {
            tempMat = mat3(orientation);  //get the rotation part from the plane's matrix

            x=(tcur->getXSpeed())/TFACTORA;
            y=(tcur->getYSpeed())/TFACTORA;
            //add cursor to queue for flicking

            newLocationVector = tempMat*vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
            newLocationMatrix = glm::translate(mat4(),newLocationVector);   //Calculate new location by translating object by motion vector

            plane.modelMatrix = newLocationMatrix*plane.modelMatrix;  //translate plane
            pho::locationMatch(selectedAsset->modelMatrix,plane.modelMatrix);  //put cursor in plane's location

            //temp.setFromOpenGLMatrix(glm::value_ptr(tempMatrix));
            //coHeart->setWorldTransform(temp);
        }
        break;
        //*********************   ROTATE  ****************************
    case rotate:
        switch (rotTechnique) {
        case screenSpace:
            if (flicker.inFlick(rotation)) { flicker.stopFlick(rotation); } //probably have come back from a pinch flick so need to stop the flick?? test without.

            if (tcur->getCursorID() == f1id) {

                p1p = p1c;
                selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*FACTORB,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*FACTORB,vec3(1,0,0)));

                p1c.x = x;
                p1c.y = y;

                flicker.addTouch(glm::vec2(tcur->getXSpeed(),tcur->getYSpeed()));
            }

            if (tcur->getCursorID() == f2id) {

                p2p = p2c;
                selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*FACTORB,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*FACTORB,vec3(1,0,0)));

                p2c.x = tcur->getX();
                p2c.y = tcur->getY();

            }


            break;
        case lockz:
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

                float ang = newAngle-referenceAngle;

                if ( glm::abs(ang) < 1) {
                    selectedAsset->rotate(glm::rotate((ang)*(-50),vec3(0,0,1)));
                }

                if(rotTechnique != lockz) {  //because they share a CASE in this switch statement
                    selectedAsset->rotate(glm::rotate(ft.x*150,vec3(0,1,0)));
                    selectedAsset->rotate(glm::rotate(ft.y*150,vec3(1,0,0)));
                }

                flicker.addRotate(newAngle-referenceAngle);

                consumed = true;
            }
            break;
        case locky:
            if (tcur->getCursorID() == f2id) {

                p2p = p2c;
                selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                //selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

                p2c.x = tcur->getX();
                p2c.y = tcur->getY();
            }
            break;
        case lockx:
            if (tcur->getCursorID() == f2id) {

                p2p = p2c;
                //selectedAsset->rotate(glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0)));
                selectedAsset->rotate(glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0)));

                p2c.x = tcur->getX();
                p2c.y = tcur->getY();
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
        if (!flicker.inFlick(translation)) {
            flicker.endFlick(mat3(orientation),translation);
            break;
        }
    case translate:
        if (!flicker.inFlick(translation)) {
        flicker.endFlick(mat3(orientation),translation);
        }
        break;
    case rotate:
        switch (rotTechnique) {
        case screenSpace:
            if (!flicker.inFlick(rotation)) {
                flicker.endFlick(mat3(orientation),rotation);
            }
            break;
        case lockx:
        case locky:
        case lockz:
        case pinch:
            rotTechnique = screenSpace;
            //std::cout << "screenSpace" << std::endl;
            if (!flicker.inFlick(pinchy)) {
                flicker.endFlick(mat3(orientation),pinchy);
            }
            break;
        }
        break;
    }

    if (verbose)
        std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void Engine::refresh(TuioTime frameTime) {
    //std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

void Engine::checkUDP() {
    mat4 newLocationMatrix;
    vec3 newLocationVector;
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
                if ((tempEvent.state() == true) && (appState == translate)) {
                    appState = rotate;
                    rotTechnique = screenSpace;
                    printf("translate --> rotate (Screenspace)");
                }
                if ((tempEvent.state() == false) && (appState == rotate)) {
                    appState = translate;
                    p1f.x = 0.5;
                    p1f.y = 0.5;
                }

                break;
            case 3:
                if ((tempEvent.state() == true) && (appState == translate)) {
                    appState = rotate;
                    rotTechnique = screenSpace;
                    printf("translate --> rotate (Screenspace)");
                }
                if ((tempEvent.state() == false) && (appState == rotate)) {
                    appState = translate;
                    p1f.x = 0.5;
                    p1f.y = 0.5;
                }

                break;
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
    //static bool started = false;
    boost::timer::cpu_times const elapsed_times(keyboardTimer.elapsed());
    double difference = elapsed_times.wall-keyboardPreviousTime.wall;

    if (difference > 800000000) { keyPressOK = true;}
    //to stop multiple keypresses
    if (keyPressOK) {
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
            btTransform temp;
            selectedAsset = &pyramidCursor;
            pyramidCursor.modelMatrix = glm::translate(0,0,0);
            temp.setFromOpenGLMatrix(glm::value_ptr(pyramidCursor.modelMatrix));
            pyramidCursor.collisionObject->setWorldTransform(temp);
            plane.modelMatrix = cursor.modelMatrix;
            flicker.stopFlick(translation);
            flicker.stopFlick(rotation);
            flicker.stopFlick(pinchy);
            appState = translate;
            //touchPoint.x=0.0f;
            //touchPoint.y=0.0f;
            keyPressOK = false;
            keyboardPreviousTime =  elapsed_times;

        }
        if (glfwGetKey(GLFW_KEY_END)) {
            perspective -=1.0;
            sr.projectionMatrix = glm::perspective(perspective, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f);
            //log("Perspective : " +perspective);
        }

        if (glfwGetKey('1') == GLFW_PRESS) {
            appState = translate;
            technique = planeCasting;
            selectionTechnique = indieSelectRelative;
            log("indieSelectRelative");
            keyPressOK = false;
            keyboardPreviousTime =  elapsed_times;
            experiment.start();
        }

        if (glfwGetKey('2') == GLFW_PRESS) {
            appState = translate;
            selectionTechnique = raySelect;
            technique = rayCasting;
            log("RayCasting");
            keyPressOK = false;
            keyboardPreviousTime =  elapsed_times;
            experiment.start();
        }

        if (glfwGetKey('3') == GLFW_PRESS) {
            pyramidCursor.setPosition(glm::vec3(0,0,0));
            plane.setPosition(glm::vec3(0,0,0));
            experiment.currentExperiment = rotationTask;
            experiment.reset();
            experiment.start();
            keyPressOK = false;
            log("Rotation Task");
            keyboardPreviousTime =  elapsed_times;
        }

        if (glfwGetKey('4') == GLFW_PRESS) {
            pyramidCursor.setPosition(glm::vec3(0,0,0));
            plane.setPosition(glm::vec3(0,0,0));
            experiment.currentExperiment = dockingTask;
            experiment.reset();
            experiment.start();
            log("Docking Task");
            keyPressOK = false;
            keyboardPreviousTime =  elapsed_times;
        }

        if (glfwGetKey('0') == GLFW_PRESS) {
            experiment.advance();
            plane.setPosition(pyramidCursor.getPosition());
            keyPressOK = false;
            keyboardPreviousTime =  elapsed_times;
        }

    }
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

    CALL_GL(glClearDepth(1.0f));
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

    pyramidCursor.drawFromLight();
    target.drawFromLight();

    // Revert for the scene.
    CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    CALL_GL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
    CALL_GL(glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y));

}

void Engine::checkSpaceNavigator() { 

#define TRSCALE 1.0f
#define RTSCALE 3.0f

    float position[6];
    unsigned char buttons[2];
    std::fill_n(position,6,0.0f);

    if (glfwGetJoystickPos( spaceNavigator, position,6) == 6 ) {
        glfwGetJoystickButtons(spaceNavigator,buttons,2);
        if (buttons[0] == GLFW_PRESS) {

            sr.viewMatrix = glm::translate(vec3(-1*position[0]*TRSCALE,0,0))*sr.viewMatrix;
            sr.viewMatrix = glm::translate(vec3(0,position[2]*TRSCALE,0))*sr.viewMatrix;
            sr.viewMatrix = glm::translate(vec3(0,0,position[1]*TRSCALE))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*-1*position[5],vec3(0,1,0))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*-1*position[4],vec3(0,0,1))*sr.viewMatrix;
            sr.viewMatrix = glm::rotate(RTSCALE*position[3],vec3(1,0,0))*sr.viewMatrix;
        }
        else {

            boost::timer::cpu_times const elapsed_times(keyboardTimer.elapsed());
            double difference = elapsed_times.wall-keyboardPreviousTime.wall;

            if (difference > 800000000)
            {
                keyPressOK = true;
            }

            //when used as footswitch
            if(((position[2] > 0.4) || (position[1] > 0.4)) && keyPressOK)
            {
                experiment.pedal = true;
                experiment.advance();
                keyPressOK = false;
                keyboardPreviousTime =  elapsed_times;
                plane.setPosition(pyramidCursor.getPosition());
            }
            else
            {
                experiment.pedal = false;
            }

            /*selectedAsset->modelMatrix = glm::translate(vec3(position[0]*TRSCALE,0,0))*selectedAsset->modelMatrix;
            selectedAsset->modelMatrix = glm::translate(vec3(0,-1*position[2]*TRSCALE,0))*selectedAsset->modelMatrix;
            selectedAsset->modelMatrix = glm::translate(vec3(0,0,-1*position[1]*TRSCALE))*selectedAsset->modelMatrix;
            selectedAsset->rotate(glm::rotate(RTSCALE*position[5],vec3(0,1,0)));
            selectedAsset->rotate(glm::rotate(RTSCALE*position[4],vec3(0,0,1)));
            selectedAsset->rotate(glm::rotate(RTSCALE*-1*position[3],vec3(1,0,0)));*/

            /*sr.light.viewMatrix = glm::translate(vec3(position[0]*TRSCALE,0,0))*sr.light.viewMatrix;
            sr.light.viewMatrix = glm::translate(vec3(0,-1*position[2]*TRSCALE,0))*sr.light.viewMatrix;
            sr.light.viewMatrix = glm::translate(vec3(0,0,-1*position[1]*TRSCALE))*sr.light.viewMatrix;

            vec4 tempPosition = sr.light.viewMatrix[3];
            sr.light.viewMatrix = glm::rotate(RTSCALE*position[5],vec3(0,1,0))*sr.light.viewMatrix;
            sr.light.viewMatrix[3] = tempPosition;

            tempPosition = sr.light.viewMatrix[3];
            sr.light.viewMatrix = glm::rotate(RTSCALE*position[5],vec3(0,0,1))*sr.light.viewMatrix;
            sr.light.viewMatrix[3] = tempPosition;

            tempPosition = sr.light.viewMatrix[3];
            sr.light.viewMatrix = glm::rotate(RTSCALE*position[5],vec3(1,0,0))*sr.light.viewMatrix;
            sr.light.viewMatrix[3] = tempPosition;*/


            /*btTransform objTrans;
            objTrans.setFromOpenGLMatrix(glm::value_ptr(selectedAsset->modelMatrix));
            coCursor->setWorldTransform(objTrans);*/
        }
    }

}

void Engine::checkMove()
{
    float position[28];
    unsigned char buttons[19];
    std::fill_n(position,28,0.0f);

    //if (glfwGetJoystickPos( moveController, position,28) == 28 )  //to get axes

        glfwGetJoystickButtons(moveController,buttons,19);
        for (auto i=0;i<16;++i) {
            if (buttons[i] == GLFW_PRESS) {
            std::cout << "Button " << i <<  " pressed " <<std::endl;
            }
        }
}

void Engine::initPhysics()
{
    btTransform temp;

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btVector3 worldAabbMin(-1000,-1000,-1000);
    btVector3 worldAabbMax(1000,1000,1000);

    btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax);

    sr.collisionWorld = new btCollisionWorld(dispatcher,broadphase,collisionConfiguration);

    sr.collisionWorld->addCollisionObject(pyramidCursor.collisionObject);

}

void Engine::checkPhysics()
{
    btTransform temp;
    temp.setFromOpenGLMatrix(glm::value_ptr(pyramidCursor.modelMatrix));
    pyramidCursor.collisionObject->setWorldTransform(temp);

}

bool Engine::rayTest(const float &normalizedX, const float &normalizedY, pho::Asset*& intersected) {

    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
        vec4 lRayStart_NDC(
            normalizedX,
            normalizedY,
            -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
            1.0f
        );
        vec4 lRayEnd_NDC(
                    normalizedX,
                    normalizedY,
            0.0,
            1.0f
        );


        // The Projection matrix goes from Camera Space to NDC.
        // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
        mat4 InverseProjectionMatrix = glm::inverse(sr.projectionMatrix);

        // The View Matrix goes from World Space to Camera Space.
        // So inverse(ViewMatrix) goes from Camera Space to World Space.
        mat4 InverseViewMatrix = glm::inverse(sr.viewMatrix);

        vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
        vec4 lRayStart_world  = InverseViewMatrix       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
        vec4 lRayEnd_camera   = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
        vec4 lRayEnd_world    = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;


        vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
        lRayDir_world = glm::normalize(lRayDir_world);

        return rayTestWorld(vec3(lRayStart_world),lRayDir_world,intersected);

}

bool Engine::rayTestWorld(const vec3 &origin,const vec3 &direction, pho::Asset*& intersected) {

        vec3 out_origin = origin;
        vec3 out_direction = direction;
        btVector3 temp;

        out_direction = out_direction*1000.0f;

        btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_direction.x, out_direction.y, out_direction.z));
        sr.collisionWorld->rayTest(btVector3(out_origin.x, out_origin.y, out_origin.z), btVector3(out_direction.x, out_direction.y, out_direction.z), RayCallback);

        if (RayCallback.hasHit()) {
            //get our asset from the collisionObject
            intersected = static_cast<pho::Asset*>(RayCallback.m_collisionObject[0].getUserPointer());

            temp = RayCallback.m_hitPointWorld;
            intersectionPoint.x = temp.x();
            intersectionPoint.y = temp.y();
            intersectionPoint.z = temp.z();
            return true;
        }
        else {return false;}
}


bool Engine::checkPolhemus(mat4 &modelMatrix) {

        boost::mutex::scoped_lock lock(ioMutex);
        //SERIAL Queue
        while(!eventQueue.isSerialEmpty()) {
                boost::array<float,7> temp = eventQueue.serialPop();

                vec3 position;
                glm::quat orientation;
                mat4 transform;

                if (appState == select)
                {
                    //std::cout << "x :" << temp[0] << "\t\t y:" << temp[2] << "\t\t z:" << temp[3] << std::endl;
                    position = vec3(temp[0]/100,temp[1]/100,temp[2]/100);

                    //position += vec3(0,-2.7f,3.8f);
                }
                else {
                    position = vec3(temp[0],temp[1],temp[2]);
                    //position += vec3(0,0,-10);
                }

                orientation.w = temp[3];
                orientation.x = temp[4];
                orientation.y = temp[5];
                orientation.z = temp[6];

                transform=glm::toMat4(orientation);

                //Rotate the matrix from the Polhemus tracker so that we can mount it on the wii-mote with the cable running towards the floor.
                //transform = transform*glm::toMat4(glm::angleAxis(-90.0f,vec3(0,0,1)));  //multiply from the right --- WRONG!!!!!! but works for the time being

                transform[3][0] = position.x; //add position to the matrix (raw, unrotated)
                transform[3][1] = position.y;
                transform[3][2] = position.z;

                modelMatrix = transform;
                polhemusMatrix = transform;

                rayOrigin = position;
                rayDirection = mat3(transform)*vec3(0,0,-1);

                return true;
        }

        lock.unlock();
        return false;
}
