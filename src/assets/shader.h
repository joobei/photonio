#ifndef SHADER_H
#define SHADER_H

#include <String>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <vector>


namespace pho {

	class Shader {
	public:
		Shader(std::string filename);
		GLuint handle;
		const char* filename;

		GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
		GLuint CreateProgram(const GLuint vert, const GLuint frag);
	};

	// Vertex Attribute Locations
	const GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2, colorLoc=3;

	// Uniform Bindings Points
	const GLuint matricesUniLoc = 1, materialUniLoc = 2, lightUniLoc = 3;
}

#endif