#pragma warning(disable: 4819)

#include "arcball.h"

glm::vec3 pho::util::getPointOnSphere(float px, float py) {
    float x,y;

    x = px/(pho::Engine::WINDOW_SIZE_X/2);
    y = py/(pho::Engine::WINDOW_SIZE_Y/2);

    x = x-1;
    y = 1-y;

    double z2 = 1 - x * x - y * y;
    double z = z2 > 0 ? glm::sqrt(z2) : 0;

    glm::vec3 p = glm::vec3(x, y, z);
    p = glm::normalize(p);
    //std::cout << "x:" << p.x << "\t y:" << p.y << "\t z:" << p.z << "\t px:" << px << "\t py:" << py << std::endl;
    return p;
}

glm::vec3 pho::util::getPointOnSphereTouch(float x, float py) {

    float y = 1-py;

    //std::cout << "GetPointOnSphereTouch: \t";

    double z2 = 1 - x * x - y * y;
    double z = z2 > 0 ? glm::sqrt(z2) : 0;

    glm::vec3 p = glm::vec3(x, y, z);
    p = glm::normalize(p);
    //std::cout << "x:" << p.x << "\t y:" << p.y << "\t z:" << p.z << "\t px:" << x << "\t py:" << py << std::endl;
    return p;
}

glm::mat4 pho::util::getRotation(float oldx, float oldy, float newx, float newy, bool touch) {

    glm::vec3 oldp,newp;

    if (touch) {
        oldp = getPointOnSphereTouch(oldx,oldy);
        newp = getPointOnSphereTouch(newx,newy);
    }
    else {
        oldp = getPointOnSphere(oldx,oldy);
        newp = getPointOnSphere(newx,newy);
    }

    glm::vec3 axis = glm::cross(oldp,newp);
    float vangle = (glm::angle(oldp,newp)/50);

    //glm::gtx::quaternion::quat rotation = glm::gtx::quaternion::angleAxis(vangle,axis.x,axis.y,axis.z);
    glm::mat4 rotation;

    rotation = glm::axisAngleMatrix(glm::vec3(axis.x,axis.y,axis.z),vangle);

    //if the sum of squares of any row are not = 1 then we don't have a rotation matrix
    if (rotation[0][0]*rotation[0][0]+rotation[0][1]*rotation[0][1]+rotation[0][2]*rotation[0][2]+rotation[0][3]*rotation[0][3]!=1) {
        return glm::mat4();
    }
    else {
        return rotation;
    }
}
