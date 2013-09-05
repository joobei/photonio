#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "asset.h"
#include "cursor.h"
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <iostream>

namespace pho {

enum experimentType {
    rotationTask,
    dockingTask,
    practice
};

class ExpManager {
public:
    ExpManager();
    void log();
    glm::vec3 start();
    bool advance();


    void setLeftWristPosition(glm::vec3 *value);
    void setLeftWristRotation(glm::vec4 *value);
    void setRightWristPosition(glm::vec3 *value);
    void setRightWristRotation(glm::vec4 *value);
    void setWandPosition(glm::vec3 *value);
    void setSphericalCursor(pho::Asset *value);


    void setCursor(pho::Cursor *value);
    void setTarget(pho::Cursor *value);
    void setRay(pho::Ray *value);
    experimentType currentExperiment;
    std::string user;
    void setUser(const std::string &value);

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
    void randomizePositions();
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> originalPositions;
    short noOfTrials;
    short currentTrial = 0;

};
}



#endif
