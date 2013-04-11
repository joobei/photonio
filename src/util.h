#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <deque>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//errorLog << err << "File :" << __FILE__ << "Line : " << __LINE__ << '\n'; \

#define ARCBALL_RADIUS 2.0f

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

class flickManager {
public:
    flickManager();
    void addTouch(glm::vec2 speeds); //adds one point to the flickManager
    void endFlick(glm::mat3 orientationSnapshot);  //the flick manager returns true if it's a flick or false if not (also resets history?)
    void stopFlick(); //stops the flying
    glm::mat4 dampenAndGiveMatrix();
    void newFlick();
    constexpr static float alpha = 0.5;
    glm::mat4 transform;
    glm::mat3 rotation;
    bool inFlick();
private:
    bool currentlyInFlick;
    std::deque<glm::vec2> touchHistory; //store a number of values
    short times;
};

}


#endif 
