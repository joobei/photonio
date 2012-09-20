#include <String>
#include "gl/glfw.h"

namespace pho {

class Shader {
    GLuint handle;
    std::string filename;

    GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
    GLuint CreateProgram(const GLuint vert, const GLuint frag);
};

}
