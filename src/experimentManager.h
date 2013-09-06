#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "asset.h"
#include "cursor.h"
#include "glm/gtx/random.hpp"
#include "glm/glm.hpp"
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <iostream>
#include <boost/random.hpp>
#include <fstream>

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

    bool advance();



    void setWandPosition(glm::vec3 *value);

    void setCursor(pho::Cursor *value);
    void setTarget(pho::Cursor *value);
    void setRay(pho::Ray *value);
    void reset();
    void start();
    experimentType currentExperiment;
    std::string user;
    void setUser(const std::string &value);
    void closeFiles();

    pho::Asset* s0,s1,s2,s3,ts0;
    unsigned char buttons[19];
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
    std::vector<std::pair<glm::vec3,short>> positions;
    std::vector<std::pair<glm::vec3,short>> originalPositions;
    std::vector<std::pair<glm::quat,short>> angles;
    std::vector<std::pair<glm::quat,short>> originalAngles;
    short noOfTrials;
    short currentTrial = 0;

    std::ofstream bigLogFile;
    std::ofstream smallLogFile;

    std::vector<std::pair<glm::vec3,short>>::iterator iterator;
    std::vector<std::pair<glm::quat,short>>::iterator angleIterator;

    glm::vec3 p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11;
    glm::vec3 magnify(glm::vec3 &tomagnify,int by);
    int currentFrame =0;

    RotateTechnique* rotateTechnique;
    Technique* technique;
    AppState* appState;
    flickState* flickstate;
};
}



#endif
