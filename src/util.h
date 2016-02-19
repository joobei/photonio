#ifndef UTIL_H
#define UTIL_H

#include <OpenGL/gl3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <deque>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"
#include "boost/timer/timer.hpp"

#define GLDEBUG

#if defined (GLDEBUG)
#define CALL_GL(exp) {                                        \
    exp;                                                          \
    unsigned int err = GL_NO_ERROR;                               \
    do{                                                           \
    err = glGetError();                                     \
    if(err != GL_NO_ERROR){                                 \
    std::cout << err << "File :" << __FILE__ << "Line : " << __LINE__ << std::endl; \
    }                                                       \
    }while(err != GL_NO_ERROR);                                  \
    }
#else
#define CALL_GL(exp) exp
#endif

namespace pho {
void log(const char *message);

enum AppState {
    select,
    translate,
    rotate,
    direct,
    clipping,
};

enum flickState {
    translation,
    rotation,
    pinchy
};

enum RotateTechnique { //rotTechnique
    screenSpace,
    pinch,
    clutch,
    lockx,
    locky,
    lockz
};

enum SelectionTechnique {
    virtualHand,
    twod,
    raySelect
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
void locationMatch(glm::mat4 &target,const glm::mat4 &source);
void displace(glm::mat4 &target, const glm::vec3 &displacement);

//manages flicking decay
class flickManager {
public:
    flickManager();
    void addTouch(glm::vec2 speeds); //adds one point to the flickManager
    void addRotate(float angle); //adds one point to the flickManager
    void endFlick(glm::mat3 orientationSnapshot, flickState state);  //the flick manager returns true if it's a flick or false if not (also resets history?)
    void stopFlick(flickState flickstate); //stops the flying
    glm::mat4 dampenAndGiveMatrix(glm::mat3 planeRotationMat);
    glm::mat4 dampenAndGivePinchMatrix();
    glm::vec2 dampenAndGiveRotationMatrix();
    void newFlick(flickState flickstate);
    glm::mat4 transform;
    glm::mat3 rotationSnapshot;
    bool inFlick(flickState flickstate);
    bool inPinchFlick();
    bool inRotationFlick();
private:
    bool currentlyInTranslateFlick;
    bool currentlyInRotateFlick;
    bool currentlyInPinchFlick;
    boost::timer::cpu_timer flickTimer;
    std::deque<glm::vec2> touchHistory; //store a number of values
    std::deque<float> angleHistory;
    glm::vec2 launchPair;  //to save the speed of the cursor on the last movement before the flick was initiated
    glm::vec2 launchAnglePair; //hold rotation for each axis upon launch
    float launchPinchAngle;
    short translateTimes;
    short rotateTimes;
    int pinchTimes;
    float decay;
};

 
}


#endif 
