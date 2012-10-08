#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <map>
#include "util.h"

namespace pho {

	class UniformAssigner
	{
	public:
		explicit UniformAssigner(int uniform_location) : location(uniform_location){}
		UniformAssigner& operator=(int data);
		UniformAssigner& operator=(float data);
		UniformAssigner& operator=(double data);
		UniformAssigner& operator=(const glm::mediump_vec2& data);
		UniformAssigner& operator=(const glm::mediump_vec3& data);
		UniformAssigner& operator=(const glm::mediump_vec4& data);
		UniformAssigner& operator=(const glm::mediump_mat4& data);
		UniformAssigner& operator=(const glm::highp_vec2& data);
		UniformAssigner& operator=(const glm::highp_vec3& data);
		UniformAssigner& operator=(const glm::highp_vec4& data);
		UniformAssigner& operator=(const glm::highp_mat4& data);
		~UniformAssigner(){ }
	private:
		int location;
	};

	class Shader {
	public:
		Shader();
		Shader(std::string filename);
		GLuint program;
		const char* filename;
		GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
		GLuint CreateProgram(const GLuint vert, const GLuint frag);
		void SetAttribute(char * name, int val);
		std::map<std::string,GLint> attributes;
		GLint getUniform(const std::string& uniform_name);
        void use();

		pho::UniformAssigner operator[](const std::string& uniform_name);
	};

	// Vertex Attribute Locations
	const GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2, colorLoc=3;

	// Uniform Bindings Points
	const GLuint matricesUniLoc = 1, materialUniLoc = 2, lightUniLoc = 3;
}




#endif
