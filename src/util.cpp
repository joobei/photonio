#include "util.h"
#include "vector3.h"

std::string pho::readTextFile(std::string filename) {
	std::fstream shaderFile(filename,std::ios::in);
	std::string shader;
	
	std::stringstream buffer;
	buffer << shaderFile.rdbuf();
    shader = buffer.str();

	return buffer.str();
}

void pho::locationMatch(glm::mat4 &target, const glm::mat4 &source) {
    target[3][0] = source[3][0];
    target[3][1] = source[3][1];
    target[3][2] = source[3][2];
    //target[3][0] = source[3][0];
}

pho::WiiButtonState::WiiButtonState() {
	a = false;
	b = false;
	power = false;
	plus = false;
	minus = false;
    home= false;
    one = false;
	two = false;
	down = false;
	up = false;
	left = false;
	right = false;
}

void pho::WiiButtonState::reset() {
	a = false;
	b = false;
	power = false;
	plus = false;
	minus = false;
	home= false;
	one = false;
	two = false;
	down = false;
	up = false;
	left = false;
	right = false;
}

pho::flickManager::flickManager() {
    transform = glm::mat4();
    currentlyInTranslateFlick = false;
    currentlyInRotateFlick = false;
    currentlyInPinchFlick = false;
    translateTimes = 0;
    rotateTimes = 0;
    translateTimes = 0;
    decay = 0.98;
}

//adds one point to the flickManager
void pho::flickManager::addTouch(glm::vec2 point){
    touchHistory.push_front(point);
    if (touchHistory.size() > 10) touchHistory.pop_back();
    //flickTimer.start(); //not being used currently
}


//adds one point to the flickManager
void pho::flickManager::addRotate(float angle){
    angleHistory.push_front(angle);
    if (touchHistory.size() > 10) touchHistory.pop_back();
    //flickTimer.restart(); //not being used currently
}

void pho::flickManager::endFlick(glm::mat3 orientationSnapshot, flickState flickstate){

    rotation = orientationSnapshot;

    switch (flickstate) {
    case translation:
        if ((glm::abs(touchHistory[0].x) > 5.0f) || (glm::abs(touchHistory[0].y) > 5.0f)) {
            translateTimes = 500;
            currentlyInTranslateFlick = true;
            launchPair.x = touchHistory[0].x/20;
            launchPair.y = touchHistory[0].y/20;
        }
        break;
    case flickState::rotation:
        if ((glm::abs(touchHistory[0].x) > 5.0f) || (glm::abs(touchHistory[0].y) > 5.0f)) {
            rotateTimes = 1000;
            currentlyInRotateFlick = true;
            launchPair.x = touchHistory[0].x;
            launchPair.y = touchHistory[0].y;
        }
        break;
    case pinchy:
        if ((glm::abs(touchHistory[0].x) > 2.0f) || (glm::abs(touchHistory[0].y) > 2.0f)) {
            pinchTimes = 1000;
            currentlyInPinchFlick = true;
            launchPinchAngle = angleHistory[0];
        }
        break;
    }

}

//resets everything
void pho::flickManager::newFlick(flickState flickstate){

    touchHistory.clear();

    switch (flickstate) {
    case translation:
        currentlyInTranslateFlick = false;
        translateTimes = 0;
        break;
    case rotate:
        currentlyInRotateFlick = false;
        rotateTimes = 0;
        break;
    case pinchy:
        currentlyInPinchFlick = false;
        pinchTimes = 0;
        break;
    }
}


bool pho::flickManager::inFlick(flickState flickstate) {

    switch (flickstate) {
    case translation:
        return currentlyInTranslateFlick;
        break;
    case rotate:
        return currentlyInRotateFlick;
        break;
    case pinchy:
        return currentlyInPinchFlick;
        break;
    }

}

void pho::flickManager::stopFlick(flickState flickstate) {

    switch (flickstate) {
    case translation:
        currentlyInTranslateFlick = false;
        translateTimes = 0;
        break;
    case rotate:
        currentlyInRotateFlick = false;
        rotateTimes = 0;
        break;
    case pinchy:
        currentlyInPinchFlick = false;
        pinchTimes = 0;
        break;
    }

}


//dampen the saved matrix and feed us the dampened value
glm::mat4 pho::flickManager::dampenAndGiveMatrix(glm::mat3 rotationMat){
    translateTimes--;
    if (translateTimes == 0) { stopFlick(translation); return glm::mat4();  //if the flick counter has come to zero just return an identity matrix
    }
    else {
        glm::mat4 newLocationMatrix;
        glm::vec3 newLocationVector;

        launchPair *= decay; //dampen vector

        newLocationVector = rotationMat*glm::vec3(launchPair.x,0,launchPair.y);  //rotate the motion vector from TUIO in the direction of the plane
        newLocationMatrix = glm::translate(glm::mat4(),newLocationVector);   //Calculate new location by translating object by motion vector

        return newLocationMatrix;
    }
}

glm::vec2 pho::flickManager::dampenAndGiveRotationMatrix(){
    rotateTimes--;
    if (rotateTimes == 0 || rotateTimes < 0) {
        stopFlick(flickState::rotation);
        return glm::vec2(0,0);  //if the flick counter has come to zero just return an identity matrix
    }
    else {
        launchPair *= decay; //dampen vector
        return launchPair;
    }
}




glm::mat4 pho::flickManager::dampenAndGivePinchMatrix(){
    pinchTimes--;
    if (pinchTimes == 0) { stopFlick(pinchy); return glm::mat4();  //if the flick counter has come to zero just return an identity matrix
    }
    else {
        launchPinchAngle *= decay; //dampen vector

        return glm::rotate((launchPinchAngle)*(-50),glm::vec3(0,0,1));
    }
}


void pho::log(const char* message)
{
     std::cout << message << std::endl;
}


