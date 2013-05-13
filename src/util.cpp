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
    currentlyInFlick = false;
    times = 0;
    decay = 0.95;
}

//adds one point to the flickManager
void pho::flickManager::addTouch(glm::vec2 point){
    touchHistory.push_front(point);
    if (touchHistory.size() > 10) touchHistory.pop_back();
    flickTimer.restart();
}

//a flick or false if not
void pho::flickManager::endFlick(glm::mat3 orientationSnapshot){

    rotation = orientationSnapshot;
    if ((glm::abs(touchHistory[0].x) > 5.0f) || (glm::abs(touchHistory[0].y) > 5.0f)) {
        times = 500;
        currentlyInFlick = true;
        launchPair.x = touchHistory[0].x/50;
        launchPair.y = touchHistory[0].y/50;
    }
}

//resets everything
void pho::flickManager::newFlick(){
    currentlyInFlick = false;
    touchHistory.clear();
    times = 0;
}

bool pho::flickManager::inFlick() {
    return currentlyInFlick;
}

void pho::flickManager::stopFlick() {
    currentlyInFlick = false;
    times = 0;
    touchHistory.clear();
}

//dampen the saved matrix and feed us the dampened value
glm::mat4 pho::flickManager::dampenAndGiveMatrix(glm::mat3 rotationMat){
    times--;
    if (times == 0) { stopFlick(); return glm::mat4();  //if the flick counter has come to zero just return an identity matrix
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


