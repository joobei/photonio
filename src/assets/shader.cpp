#pragma warning(disable: 4819)

#include "shader.h"

using namespace boost::filesystem;

pho::UniformAssigner& pho::UniformAssigner::operator=(int data) { CALL_GL(glUniform1i(location,data)); return (*this); }
pho::UniformAssigner& pho::UniformAssigner::operator=(float data) { CALL_GL(glUniform1f(location,data)); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec2& data){ CALL_GL(glUniform2f(location,data.x,data.y)); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec3& data){ CALL_GL(glUniform3f(location,data.x,data.y,data.z)); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::vec4& data){ CALL_GL(glUniform4f(location,data.x,data.y,data.z,data.w)); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::mat4& data){ CALL_GL(glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(data))); return (*this);}
pho::UniformAssigner& pho::UniformAssigner::operator=(const glm::mat3& data){ CALL_GL(glUniformMatrix3fv(location,1,GL_FALSE,glm::value_ptr(data))); return (*this);}

pho::Shader::Shader() {
	vertex = fragment = geometry = 0;
}

pho::Shader::Shader(std::string filename) {
	vertex = CreateShader(GL_VERTEX_SHADER,pho::readTextFile(filename+".vert"));
	fragment = CreateShader(GL_FRAGMENT_SHADER,pho::readTextFile(filename+".frag"));
	if (exists(filename+".geom")) {
	geometry = CreateShader(GL_GEOMETRY_SHADER,pho::readTextFile(filename+".geom"));
	program = CreateProgram(vertex, fragment, geometry);
	}
	else {
		program = CreateProgram(vertex, fragment);
	}
}

GLuint pho::Shader::CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	CALL_GL(glShaderSource(shader, 1, &strFileData, NULL));

	CALL_GL(glCompileShader(shader));

	GLint status;
	CALL_GL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		CALL_GL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength));

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		CALL_GL(glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog));

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;

		return 0;
	}

	return shader;
}

GLuint pho::Shader::CreateProgram(const GLuint vert, const GLuint frag, const GLuint geom) {
	GLuint program = glCreateProgram();

	glAttachShader(program, vert);
	glAttachShader(program, frag);
	if (geom !=0) {
	glAttachShader(program, geom);
	}

	CALL_GL(glBindAttribLocation(program,vertexLoc,"in_Position"));
	CALL_GL(glBindAttribLocation(program,colorLoc,"in_Color"));
	CALL_GL(glBindAttribLocation(program,texCoordLoc,"in_TexCoord"));
	CALL_GL(glBindAttribLocation(program,normalLoc,"in_Normal"));

	CALL_GL(glLinkProgram(program));

	GLint status;
	CALL_GL(glGetProgramiv (program, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		CALL_GL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength));

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		CALL_GL(glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog));
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	return program;
}

GLuint pho::Shader::CreateProgram(const GLuint vert, const GLuint frag) {
	GLuint program = glCreateProgram();

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	CALL_GL(glBindAttribLocation(program,vertexLoc,"in_Position"));
	CALL_GL(glBindAttribLocation(program,colorLoc,"in_Color"));
	CALL_GL(glBindAttribLocation(program,texCoordLoc,"in_TexCoord"));
	CALL_GL(glBindAttribLocation(program,normalLoc,"in_Normal"));

	CALL_GL(glLinkProgram(program));

	GLint status;
	CALL_GL(glGetProgramiv (program, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		CALL_GL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength));

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		CALL_GL(glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog));
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
    CALL_GL(glUseProgram(program));
}
