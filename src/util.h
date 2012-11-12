#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>
#include "glm\glm.hpp"

//errorLog << err << "File :" << __FILE__ << "Line : " << __LINE__ << '\n'; \

#define GLDEBUG

#if defined (GLDEBUG)
#define CALL_GL(exp) {                                        \
    exp;                                                          \
    unsigned int err = GL_NO_ERROR;                               \
    do{                                                           \
    err = glGetError();                                     \
    if(err != GL_NO_ERROR){                                 \
    std::cout << err << "File :" << __FILE__ << "Line : " << __LINE__ << '\n'; \
    }                                                       \
    }while(err != GL_NO_ERROR);                                  \
    }
#else
#define CALL_GL(exp) exp
#endif

#define ARCBALL_RADIUS 0.5f

namespace pho {	

	class WiiButtonState {
public:
    WiiButtonState();
    bool a,b,power,plus,minus,home,one,two,down,up,left,right;
    void reset();
};

    struct Buttons {
        WiiButtonState wiimote;
        bool mouse1, mouse2;
        bool snButton1,snButton2;
        bool phoneVolumeUp,phoneVolumeDown,phoneButtonTouch;
    };

enum InputState { //appInputState
    idle,
    translate,
    rotate
};

enum RotateTechnique { //rotTechnique
    singleAxis,
    pinch2,
    screenSpace,
    pinch,
    trackBall
};

enum Technique {  //appmode
    planeCasting,
    rayCasting,
    spaceNavigator,
    mouse
};


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

std::string readTextFile(std::string filename);

}


#endif 
