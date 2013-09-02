#include "experimentManager.h"

pho::Ray *pho::ExpManager::getRay() const
{
return ray;
}

void pho::ExpManager::setRay(pho::Ray *value)
{
ray = value;
}

pho::Cursor *pho::ExpManager::getTarget() const
{
return target;
}

void pho::ExpManager::setTarget(pho::Cursor *value)
{
target = value;
}

pho::Cursor *pho::ExpManager::getCursor() const
{
return cursor;
}

void pho::ExpManager::setCursor(pho::Cursor *value)
{
cursor = value;
}

pho::Asset *pho::ExpManager::getSphericalCursor() const
{
return sphericalCursor;
}

void pho::ExpManager::setSphericalCursor(pho::Asset *value)
{
sphericalCursor = value;
}

glm::vec3 *pho::ExpManager::getWandPosition() const
{
return wandPosition;
}

void pho::ExpManager::setWandPosition(glm::vec3 *value)
{
wandPosition = value;
}

glm::vec4 *pho::ExpManager::getRightWristRotation() const
{
return rightWristRotation;
}

void pho::ExpManager::setRightWristRotation(glm::vec4 *value)
{
rightWristRotation = value;
}

glm::vec3 *pho::ExpManager::getRightWristPosition() const
{
return rightWristPosition;
}

void pho::ExpManager::setRightWristPosition(glm::vec3 *value)
{
rightWristPosition = value;
}

glm::vec4 *pho::ExpManager::getLeftWristRotation() const
{
return leftWristRotation;
}

void pho::ExpManager::setLeftWristRotation(glm::vec4 *value)
{
leftWristRotation = value;
}

glm::vec3 *pho::ExpManager::getLeftWristPosition() const
{
return leftWristPosition;
}

void pho::ExpManager::setLeftWristPosition(glm::vec3 *value)
{
leftWristPosition = value;
}


glm::vec3 pho::ExpManager::advance()
{
    float x = -10 + (float)rand()/((float)RAND_MAX/(10-(-10)));
    float y = -10 + (float)rand()/((float)RAND_MAX/(10-(-10)));
    float z = -10 + (float)rand()/((float)RAND_MAX/(0.1-(-0.1)));

    return glm::vec3(x,y,z);
}


pho::ExpManager::ExpManager()
{

}
