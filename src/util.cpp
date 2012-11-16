#pragma warning(disable: 4819)
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
