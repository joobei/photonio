#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "asset.h"
#include "plane.h"
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

struct finger {
    bool exists = false;
    float x,y;
    int id;
};

enum experimentType {
    rotationTask,
    dockingTask,
    movementTask,
    practice
};

class ExpManager {
public:
    ExpManager();
    void log();
    bool advance();
    void setCursor(pho::Cursor *value);
    void setTarget(pho::Cursor *value);
    void setPlane(pho::Plane *value);
    void reset();
    void start();
    void restartTrial();
    experimentType currentExperiment;
    std::string user;
    void setUser(const std::string &value);
    void closeFiles();
    finger finger1;
    finger finger2;
    pho::Asset* s0,s1,s2,s3,ts0;
    unsigned char buttons[19];
    bool pedal;
    glm::mat4* wandPosition;
    pho::Cursor* cursor;
    pho::Cursor* target;
    pho::Plane* plane;
    pho::Ray* ray;

    void randomizePositions();
    std::vector<std::pair<glm::vec3,short>> positions;
    std::vector<std::pair<glm::vec3,short>> originalPositions;
    std::vector<std::pair<glm::quat,short>> angles;
    std::vector<std::pair<glm::quat,short>> originalAngles;
    short noOfTrials;
    short currentTrial = 0;

    std::vector<std::pair<glm::vec3,short>>::iterator iterator;
    std::vector<std::pair<glm::quat,short>>::iterator angleIterator;

    glm::vec3 p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11;
    glm::vec3 magnify(glm::vec3 &tomagnify,int by);
    int currentFrame =0;

    RotateTechnique* rotateTechnique;
    Technique* technique;
    AppState* appState;

    short inFlick = 0;

    boost::timer::cpu_timer trialTimer;
    boost::timer::cpu_timer experimentTimer;
    boost::timer::cpu_timer sessionTimer;

    boost::timer::cpu_times trialTime;
    boost::timer::cpu_times experimentTime;
    boost::timer::cpu_times sessionTime;

    bool started = false;

    glm::vec3 polhemus1pos;
    glm::vec4 polhemus1quat;

    glm::vec3 polhemus2pos;
    glm::vec4 polhemus2quat;

    glm::mat4* phoneMatrix;

    std::ofstream bigLogFile;
    std::ofstream smallLogFile;

    std::ofstream fatigueLogFile;
    std::ofstream touchLogFile;

};
}



#endif
