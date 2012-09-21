#ifndef SHADER_H
#define SHADER_H

#include <String>
#include "gl/glfw.h"

namespace pho {

	class Shader {
	public:
		GLuint handle;
		std::string filename;

		GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
		GLuint CreateProgram(const GLuint vert, const GLuint frag);
	};

}

#endif