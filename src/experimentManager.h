#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "asset.h"
#include "cursor.h"
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace pho {

class ExpManager {
public:
    ExpManager();
    void log();
    glm::vec3 start();
    glm::vec3 advance();

    glm::vec3 *getLeftWristPosition() const;
    void setLeftWristPosition(glm::vec3 *value);

    glm::vec4 *getLeftWristRotation() const;
    void setLeftWristRotation(glm::vec4 *value);

    glm::vec3 *getRightWristPosition() const;
    void setRightWristPosition(glm::vec3 *value);

    glm::vec4 *getRightWristRotation() const;
    void setRightWristRotation(glm::vec4 *value);

    glm::vec3 *getWandPosition() const;
    void setWandPosition(glm::vec3 *value);

    pho::Asset *getSphericalCursor() const;
    void setSphericalCursor(pho::Asset *value);

    pho::Cursor *getCursor() const;
    void setCursor(pho::Cursor *value);

    pho::Cursor *getTarget() const;
    void setTarget(pho::Cursor *value);

    pho::Ray *getRay() const;
    void setRay(pho::Ray *value);

private:
    glm::vec3* leftWristPosition;
    glm::vec4* leftWristRotation;
    glm::vec3* rightWristPosition;
    glm::vec4* rightWristRotation;
    glm::vec3* wandPosition;
    pho::Asset* sphericalCursor;
    pho::Cursor* cursor;
    pho::Cursor* target;
    pho::Ray* ray;
};
}






#endif
