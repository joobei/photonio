#include "shader.h"

pho::Shader::Shader(std::string filename) {
    GLuint vertex,fragment;
    vertex = CreateShader(GL_VERTEX_SHADER,filename+".vert");
    fragment = CreateShader(GL_FRAGMENT_SHADER,filename+".frag");
    handle = pho::Shader::CreateProgram(vertex, fragment);
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