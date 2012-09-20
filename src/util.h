#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>
#include "box.h"
#include "glm\glm.hpp"

namespace pho {

	// Vertex Attribute Locations
	const GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2, colorLoc=3;

	// Uniform Bindings Points
	const GLuint matricesUniLoc = 1, materialUniLoc = 2, lightUniLoc = 3;

	// The sampler uniform for textured models
	// we are assuming a single texture so this will
	//always be texture unit 0
	const GLuint texUnit = 0;


	//return the angle C of a triangle formed by 3 points
	inline float sssTriangleC(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc) {
		float a = glm::distance(pc,pb);
		float b = glm::distance(pc,pa);
		float c = glm::distance(pa,pb);

		float cosC = (a*a+b*b-c*c)/(2*a*b);

		return glm::acos(cosC);
	}

	inline std::string readTextFile(const char* filename) {
		std::fstream shaderFile(filename,std::ios::in);
		std::string shader;

		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shader = buffer.str();

		return buffer.str();
	}

	class WiiButtonState {
	public:
		WiiButtonState();
		bool a,b,power,plus,minus,home,one,two,down,up,left,right;
		void reset();
	};
}


#endif 
