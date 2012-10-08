#pragma warning(disable: 4819)

#include "shader.h"

pho::UniformAssigner& pho::UniformAssigner::operator=(int data) { glUniform1i(location,data); return (*this); }
pho::UniformAssigner& pho::UniformAssigner::operator=(float data) { glUniform1f(location,data); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec2& data){ glUniform2f(location,data.x,data.y); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec3& data){ glUniform3f(location,data.x,data.y,data.z); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec4& data){ glUniform4f(location,data.x,data.y,data.z,data.w); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::mat4& data){ glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(data)); return (*this);}

pho::Shader::Shader() {

}

pho::Shader::Shader(std::string filename) {
	GLuint vertex,fragment;
	vertex = CreateShader(GL_VERTEX_SHADER,filename+".vert");
	fragment = CreateShader(GL_FRAGMENT_SHADER,filename+".frag");
	program = CreateProgram(vertex, fragment);
}

GLuint pho::Shader::CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint pho::Shader::CreateProgram(const GLuint vert, const GLuint frag) {
	GLuint program = glCreateProgram();

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glBindAttribLocation(program,vertexLoc,"in_Position");
	glBindAttribLocation(program,colorLoc,"in_Color");
	glBindAttribLocation(program,texCoordLoc,"in_TexCoord");
	glBindAttribLocation(program,normalLoc,"in_Normal");

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	return program;
}

//Get value of uniform, adds it if it doesn't exist.
GLint pho::Shader::getUniform(const std::string& uniform_name) {
	std::map<std::string,GLint>::iterator it;


	if (attributes.find(uniform_name) == attributes.end()) { 
		//hasn't been found so add it
		GLint temp = glGetUniformLocation(program,uniform_name.c_str());
		
		attributes.insert(std::pair<std::string,GLint>(uniform_name,temp));
		return temp;
	}
	else { 
		//has been found so return the value
		it = attributes.find(uniform_name);
		return it->second;
	}
}

pho::UniformAssigner pho::Shader::operator[](const std::string& uniform_name)
{
	return UniformAssigner(getUniform(uniform_name));
}

void pho::Shader::use() {
    glUseProgram(program);
}
