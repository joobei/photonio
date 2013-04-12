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
}

//adds one point to the flickManager
void pho::flickManager::addTouch(glm::vec2 point){
    touchHistory.push_front(point);
    if (touchHistory.size() > 10) touchHistory.pop_back();
}

//a flick or false if not
void pho::flickManager::endFlick(glm::mat3 orientationSnapshot){

    rotation = orientationSnapshot;
    if ((glm::abs(touchHistory[0].x) > 2.0f) || (glm::abs(touchHistory[0].y) > 2.0f)) {
        times = 20;
        currentlyInFlick = true; }
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
glm::mat4 pho::flickManager::dampenAndGiveMatrix(){
    times--;
    if (times < 0) { stopFlick(); return glm::mat4();
    }
    else {
        glm::vec2 temp = glm::vec2(touchHistory[0].x,touchHistory[0].y);

        glm::mat4 newLocationMatrix;
        glm::vec3 newLocationVector;

        newLocationVector = rotation*glm::vec3(temp.x,0,temp.y);  //rotate the motion vector from TUIO in the direction of the plane
        newLocationMatrix = glm::translate(glm::mat4(),newLocationVector);   //Calculate new location by translating object by motion vector

        return newLocationMatrix/((20-times)*alpha);
    }
}
