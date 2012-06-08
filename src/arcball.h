#ifndef ARCBALL_H
#define ARCBALL_H

#include "photonio.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/matrix_interpolation.hpp"


namespace pho {
 namespace util {

glm::vec3 getPointOnSphere(int x, int y);

glm::vec3 getPointOnSphereTouch(float x, float y);

glm::mat4 getRotation(float oldx, float oldy, float newx, float newy, bool touch);

 }


}

#endif
