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
    inFlick = false;
}

//adds one point to the flickManager
void pho::flickManager::addTouch(glm::vec2 point){
    touchHistory.push_front(point);

    if (touchHistory.size() > 10) {
        std::deque<glm::vec2>::iterator it = touchHistory.end();
        touchHistory.erase(it);
    }
}

//the flick manager returns true if it's a flick or false if not (also resets history?)
bool pho::flickManager::startflick(){

    if (touchHistory.size() > 1) {

        glm::vec2 p2 = touchHistory.at(0);
        glm::vec2 p1 = touchHistory.at(1);

        if (glm::distance(p2,p1) > 0.01 ) {
            touchHistory.clear();
            return true;
        }
        else return false;
    }
    return false;
}

//resets everything
void pho::flickManager::stop(){
    touchHistory.clear();
}

//dampen the saved matrix and feed us the dampened value
glm::mat4 pho::flickManager::dampenAndGiveMatrix(){
    glm::mat4 newLocationMatrix;
    glm::vec3 newLocationVector;
    newLocationVector = tempMat*vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
    newLocationMatrix = glm::translate(mat4(),newLocationVector);   //Calculate new location by translating object by motion vector
    transform *= 0.1;
    return transform;
}
