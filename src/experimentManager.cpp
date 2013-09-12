#include "experimentManager.h"
using namespace std;
using namespace boost;

pho::ExpManager::ExpManager()
{
    experimentTimer.start();
    experimentTime = experimentTimer.elapsed();

    p0 = glm::normalize(glm::vec3(0.000000,-1.000000,0.000000));
    p1= glm::normalize(glm::vec3(0.723600,-0.447215 ,0.525720));
    p2 = glm::normalize(glm::vec3(-0.276385, -0.447215, 0.850640));
    p3 = glm::normalize(glm::vec3(-0.894425, -0.447215, 0.000000));
    p4 = glm::normalize(glm::vec3(-0.276385, -0.447215, -0.850640));
    p5 = glm::normalize(glm::vec3(0.723600, -0.447215, -0.525720));
    p6 = glm::normalize(glm::vec3(0.276385, 0.447215 ,0.850640));
    p7 = glm::normalize(glm::vec3(-0.723600, 0.447215, 0.525720));
    p8 = glm::normalize(glm::vec3(-0.723600, 0.447215, -0.525720));
    p9 = glm::normalize(glm::vec3(0.276385, 0.447215, -0.850640));
    p10 = glm::normalize(glm::vec3(0.894425, 0.447215, 0.000000));
    p11 = glm::normalize(glm::vec3(0.000000, 1.000000, 0.000000));

    /*originalPositions.push_back(std::make_pair(p0,1));
    originalPositions.push_back(std::make_pair(p1,2));
    originalPositions.push_back(std::make_pair(p2,3));
    originalPositions.push_back(std::make_pair(p3,4));
    originalPositions.push_back(std::make_pair(p4,5));
    originalPositions.push_back(std::make_pair(p5,6));
    originalPositions.push_back(std::make_pair(p6,7));
    originalPositions.push_back(std::make_pair(p7,8));
    originalPositions.push_back(std::make_pair(p8,9));
    originalPositions.push_back(std::make_pair(p9,10));
    originalPositions.push_back(std::make_pair(p10,11));
    originalPositions.push_back(std::make_pair(p11,12));*/

    short multiplier = 2;

    originalPositions.push_back(std::make_pair(magnify(p0,multiplier),1));
    originalPositions.push_back(std::make_pair(magnify(p1,multiplier),2));
    originalPositions.push_back(std::make_pair(magnify(p2,multiplier),3));
    originalPositions.push_back(std::make_pair(magnify(p3,multiplier),4));
    originalPositions.push_back(std::make_pair(magnify(p4,multiplier),5));
    originalPositions.push_back(std::make_pair(magnify(p5,multiplier),6));
    originalPositions.push_back(std::make_pair(magnify(p6,multiplier),7));
    originalPositions.push_back(std::make_pair(magnify(p7,multiplier),8));
    originalPositions.push_back(std::make_pair(magnify(p8,multiplier),9));
    originalPositions.push_back(std::make_pair(magnify(p9,multiplier),10));
    originalPositions.push_back(std::make_pair(magnify(p10,multiplier),11));
    originalPositions.push_back(std::make_pair(magnify(p11,multiplier),12));

    multiplier = 4;

    originalPositions.push_back(std::make_pair(magnify(p0,multiplier),12+1));
    originalPositions.push_back(std::make_pair(magnify(p1,multiplier),12+2));
    originalPositions.push_back(std::make_pair(magnify(p2,multiplier),12+3));
    originalPositions.push_back(std::make_pair(magnify(p3,multiplier),12+4));
    originalPositions.push_back(std::make_pair(magnify(p4,multiplier),12+5));
    originalPositions.push_back(std::make_pair(magnify(p5,multiplier),12+6));
    originalPositions.push_back(std::make_pair(magnify(p6,multiplier),12+7));
    originalPositions.push_back(std::make_pair(magnify(p7,multiplier),12+8));
    originalPositions.push_back(std::make_pair(magnify(p8,multiplier),12+9));
    originalPositions.push_back(std::make_pair(magnify(p9,multiplier),12+10));
    originalPositions.push_back(std::make_pair(magnify(p10,multiplier),12+11));
    originalPositions.push_back(std::make_pair(magnify(p11,multiplier),12+12));

    for (auto i=0;i<2;++i) {
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,0,0))),1));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,0,1))),2));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,1,0))),3));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,1,1))),4));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,0,0))),5));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,0,1))),6));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,1,0))),7));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,1,1))),8));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,0,0))),9));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,0,1))),10));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,1,0))),11));
        originalAngles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(1,1,0))),12));
    }

    currentExperiment = practice;
    noOfTrials = originalPositions.size();

    bigLogFile.open ("logs/bigFile.csv", std::ofstream::out | std::ofstream::app);
    smallLogFile.open ("logs/smallFile.csv", std::ofstream::out | std::ofstream::app);
    fatigueLogFile.open("logs/fatigue.csv", std::ofstream::out | std::ofstream::app);
    touchLogFile.open("logs/touch.csv", std::ofstream::out | std::ofstream::app);

    positions = originalPositions;
    angles = originalAngles;

    iterator = positions.begin();
    angleIterator = angles.begin();

    sessionTimer.stop();
    trialTimer.stop();
    //experimentTimer.stop();
}

void pho::ExpManager::setPlane(pho::Plane *value)
{
    plane = value;
}

void pho::ExpManager::reset()
{
    currentTrial = 1;
    randomizePositions();
    iterator = positions.begin();
    angleIterator = angles.begin();
    started = false;
    trialTimer.stop();
}

void pho::ExpManager::start()
{
    glm::vec3 pos;
    glm::mat4 loc;

    sessionTimer.start();
    sessionTime = sessionTimer.elapsed();
    trialTimer.start();
    trialTime = trialTimer.elapsed();

    currentTrial = 1;
    iterator = positions.begin();
    angleIterator = angles.begin();

    switch (currentExperiment) {
    case dockingTask:
        pos = iterator->first;
        loc = glm::toMat4(angleIterator->first);
        loc[3] = glm::vec4(pos,1);
        target->modelMatrix = loc;
        cursor->modelMatrix = glm::mat4();
        plane->modelMatrix = glm::mat4();
        break;
    case rotationTask:
        target->modelMatrix = glm::toMat4(angleIterator->first);
        cursor->modelMatrix = glm::mat4();
        break;
    case movementTask:
        target->setPosition(iterator->first);
        cursor->modelMatrix = glm::mat4();
        plane->modelMatrix = glm::mat4();
        break;
    }

    started = true;
}

void pho::ExpManager::restartTrial()
{
    trialTime = trialTimer.elapsed();
}

void pho::ExpManager::setUser(const std::string &value)
{
    user = value;
}

void pho::ExpManager::closeFiles()
{
    bigLogFile.close();
    smallLogFile.close();

    fatigueLogFile.close();
    touchLogFile.close();
}

void pho::ExpManager::log()
{

    boost::timer::cpu_times const elapsed_time(trialTimer.elapsed());
    double difference = elapsed_time.wall-trialTime.wall;

    boost::timer::cpu_times const elapsed_session(sessionTimer.elapsed());
    double differenceSession = elapsed_session.wall-sessionTime.wall;

    boost::timer::cpu_times const elapsed_experiment(experimentTimer.elapsed());
    double differenceExperiment = elapsed_experiment.wall-experimentTime.wall;

    glm::quat cur = glm::toQuat(cursor->modelMatrix);
    glm::quat tar = glm::toQuat(target->modelMatrix);


    if (started) {

        bigLogFile << user << ';';
        bigLogFile << currentFrame++ << ';';
        bigLogFile << currentTrial << ';';
        bigLogFile << currentExperiment << ';';
        switch (*rotateTechnique) {
        case 0:bigLogFile << "screenspace" << ';';break;
        case 1:bigLogFile << "pinch" << ';';break;
        case 2:bigLogFile << "clutch" << ';';break;
        case 3:bigLogFile << "lockx" << ';';break;
        case 4:bigLogFile << "locky" << ';';break;
        case 5:bigLogFile << "lockz" << ';';break;
        }

        switch (*technique) {
        case 0:bigLogFile << "planecasting" << ';';break;
        case 1:bigLogFile << "raycasting" << ';';break;
        case 2:bigLogFile << "spacenavigator" << ';';break;
        }

        switch (*appState) {
        case 0:bigLogFile << "select" << ';';break;
        case 1:bigLogFile << "intersect" << ';';break;
        case 2:bigLogFile << "translate" << ';';break;
        case 3:bigLogFile << "rotate" << ';';break;
        case 4:bigLogFile << "direct" << ';';break;
        }
        bigLogFile << inFlick << ';';

        touchLogFile << user << ';';
        touchLogFile << currentFrame++ << ';';
        touchLogFile << currentTrial << ';';
        switch (currentExperiment) {
        case 0:touchLogFile << "rotationTask" << ';';break;
        case 1:touchLogFile << "dockingTask" << ';';break;
        case 2:touchLogFile << "movementTask" << ';';break;
        }
        switch (*rotateTechnique) {
        case 0:touchLogFile << "screenspace" << ';';break;
        case 1:touchLogFile << "pinch" << ';';break;
        case 2:touchLogFile << "clutch" << ';';break;
        case 3:touchLogFile << "lockx" << ';';break;
        case 4:touchLogFile << "locky" << ';';break;
        case 5:touchLogFile << "lockz" << ';';break;
        }

        switch (*technique) {
        case 0:touchLogFile << "planecasting" << ';';break;
        case 1:touchLogFile << "raycasting" << ';';break;
        case 2:touchLogFile << "spacenavigator" << ';';break;
        }

        switch (*appState) {
        case 0:touchLogFile << "select" << ';';break;
        case 1:touchLogFile << "intersect" << ';';break;
        case 2:touchLogFile << "translate" << ';';break;
        case 3:touchLogFile << "rotate" << ';';break;
        case 4:touchLogFile << "direct" << ';';break;
        }

        touchLogFile << inFlick << ';';
        touchLogFile << pedal << ';';

        fatigueLogFile << user << ';';
        fatigueLogFile << currentFrame++ << ';';
        fatigueLogFile << currentTrial << ';';
        switch (currentExperiment) {
        case 0:fatigueLogFile << "rotationTask" << ';';break;
        case 1:fatigueLogFile << "dockingTask" << ';';break;
        case 2:fatigueLogFile << "movementTask" << ';';break;
        }
        switch (*rotateTechnique) {
        case 0:fatigueLogFile << "screenspace" << ';';break;
        case 1:fatigueLogFile << "pinch" << ';';break;
        case 2:fatigueLogFile << "clutch" << ';';break;
        case 3:fatigueLogFile << "lockx" << ';';break;
        case 4:fatigueLogFile << "locky" << ';';break;
        case 5:fatigueLogFile << "lockz" << ';';break;
        }

        switch (*technique) {
        case 0:fatigueLogFile << "planecasting" << ';';break;
        case 1:fatigueLogFile << "raycasting" << ';';break;
        case 2:fatigueLogFile << "spacenavigator" << ';';break;
        }

        switch (*appState) {
        case 0:fatigueLogFile << "select" << ';';break;
        case 1:fatigueLogFile << "intersect" << ';';break;
        case 2:fatigueLogFile << "translate" << ';';break;
        case 3:fatigueLogFile << "rotate" << ';';break;
        case 4:fatigueLogFile << "direct" << ';';break;
        }
        fatigueLogFile << pedal << ';';

        bigLogFile << iterator->second << ';';
        bigLogFile << angleIterator->second << ';';
        bigLogFile << cursor->getPosition().x << ';';
        bigLogFile << cursor->getPosition().y << ';';
        bigLogFile << cursor->getPosition().z << ';';
        bigLogFile << cur.x << ';';
        bigLogFile << cur.y << ';';
        bigLogFile << cur.z << ';';
        bigLogFile << cur.w << ';';
        bigLogFile << target->getPosition().x << ';';
        bigLogFile << target->getPosition().y << ';';
        bigLogFile << target->getPosition().z << ';';
        bigLogFile << tar.x << ';';
        bigLogFile << tar.y << ';';
        bigLogFile << tar.z << ';';
        bigLogFile << tar.w << ';';

        bigLogFile << wandrotate << ';';
        bigLogFile << wandtranslate << ';';

        touchLogFile << finger1.exists << ';';
        touchLogFile << finger1.id << ';';
        touchLogFile << finger1.x << ';';
        touchLogFile << finger1.y << ';';
        touchLogFile << finger2.exists << ';';
        touchLogFile << finger2.id << ';';
        touchLogFile << finger2.x << ';';
        touchLogFile << finger2.y << ';';

        fatigueLogFile << polhemus1pos.x << ';';
        fatigueLogFile << polhemus1pos.y << ';';
        fatigueLogFile << polhemus1pos.z << ';';
        fatigueLogFile << polhemus1quat.x << ';';
        fatigueLogFile << polhemus1quat.y << ';';
        fatigueLogFile << polhemus1quat.z << ';';
        fatigueLogFile << polhemus1quat.w << ';';
        fatigueLogFile << polhemus2pos.x << ';';
        fatigueLogFile << polhemus2pos.y << ';';
        fatigueLogFile << polhemus2pos.z << ';';
        fatigueLogFile << polhemus2quat.x << ';';
        fatigueLogFile << polhemus2quat.y << ';';
        fatigueLogFile << polhemus2quat.z << ';';
        fatigueLogFile << polhemus2quat.w << ';';

        glm::quat phone = glm::normalize(glm::toQuat(*phoneMatrix));

        fatigueLogFile << phone.x << ';';
        fatigueLogFile << phone.y << ';';
        fatigueLogFile << phone.z << ';';
        fatigueLogFile << phone.w << ';';

        bigLogFile << pedal << ';';
        bigLogFile << difference/1000000.0f << ';';
        bigLogFile << differenceSession/1000000.0f << ';';
        bigLogFile << differenceExperiment/1000000.0f << ';';
        //add APP STATES
        //flick states
        bigLogFile << '\n';
        touchLogFile << '\n';
        fatigueLogFile << '\n';

    }
}


bool pho::ExpManager::advance()
{
    float x,y,z;
    glm::vec3 pos;
    glm::mat4 loc;

    boost::timer::cpu_times const elapsed_times(trialTimer.elapsed());
    double difference = elapsed_times.wall-trialTime.wall;

    boost::timer::cpu_times const elapsed_session(sessionTimer.elapsed());
    double differenceSession = elapsed_session.wall-sessionTime.wall;

    boost::timer::cpu_times const elapsed_experiment(experimentTimer.elapsed());
    double differenceExperiment = elapsed_experiment.wall-experimentTime.wall;

    glm::quat cur = glm::toQuat(cursor->modelMatrix);
    glm::quat tar = glm::toQuat(target->modelMatrix);

    if (started) {
        smallLogFile << user << ';';
        smallLogFile << currentFrame << ';';

        //smallLogFile << difference/1000000.0f << ';';
        smallLogFile << difference/1000000.0f << ';';
        smallLogFile << differenceSession/1000000.0f << ';';
        smallLogFile << differenceExperiment/1000000.0f << ';';

        switch (currentExperiment) {
        case 0:smallLogFile << "rotationTask" << ';';break;
        case 1:smallLogFile << "dockingTask" << ';';break;
        case 2:smallLogFile << "movementTask" << ';';break;
        }

        smallLogFile << currentTrial << ';';

        switch (*rotateTechnique) {
        case 0:smallLogFile << "screenspace" << ';';break;
        case 1:smallLogFile << "pinch" << ';';break;
        case 2:smallLogFile << "clutch" << ';';break;
        case 3:smallLogFile << "lockx" << ';';break;
        case 4:smallLogFile << "locky" << ';';break;
        case 5:smallLogFile << "lockz" << ';';break;
        }

        switch (*technique) {
        case 0:smallLogFile << "planecasting" << ';';break;
        case 1:smallLogFile << "raycasting" << ';';break;
        case 2:smallLogFile << "spacenavigator" << ';';break;
        }

        switch (*appState) {
        case 0:smallLogFile << "select" << ';';break;
        case 1:smallLogFile << "intersect" << ';';break;
        case 2:smallLogFile << "translate" << ';';break;
        case 3:smallLogFile << "rotate" << ';';break;
        case 4:smallLogFile << "direct" << ';';break;
        }

        smallLogFile << inFlick << ';';
        smallLogFile << pedal << ';';

        smallLogFile << iterator->second << ';';
        smallLogFile << angleIterator->second << ';';
        smallLogFile << cursor->getPosition().x << ';';
        smallLogFile << cursor->getPosition().y << ';';
        smallLogFile << cursor->getPosition().z << ';';
        smallLogFile << cur.x << ';';
        smallLogFile << cur.y << ';';
        smallLogFile << cur.z << ';';
        smallLogFile << cur.w << ';';
        smallLogFile << target->getPosition().x << ';';
        smallLogFile << target->getPosition().y << ';';
        smallLogFile << target->getPosition().z << ';';
        smallLogFile << tar.x << ';';
        smallLogFile << tar.y << ';';
        smallLogFile << tar.z << ';';
        smallLogFile << tar.w << ';';


        smallLogFile << '\n';

    }

    std::srand(time(NULL));
    switch (currentExperiment) {
    case practice:
        x = -5 + (float)rand()/((float)RAND_MAX/(5-(-5)));
        y = -5 + (float)rand()/((float)RAND_MAX/(5-(-5)));
        z = -5 + (float)rand()/((float)RAND_MAX/(5-(-5)));
        target->setPosition(glm::vec3(x,y,z));
        break;
    case movementTask:
        if(iterator != positions.end())
        {
            ++currentTrial;
            ++iterator;
            ++angleIterator;
        }
        if(iterator != positions.end())
        {
            target->modelMatrix = glm::mat4();
            target->setPosition(iterator->first);
            //loc = glm::toMat4(angleIterator->first);
            //loc[3] = glm::vec4(pos,1);
            //target->modelMatrix = loc;
            cursor->modelMatrix = glm::mat4();
        }
        if(iterator == positions.end())
        {
            target->setPosition(glm::vec3(5,0,-530));
            std::cout << "movement Experiment Finished";
            sessionTimer.stop();
            trialTimer.stop();
            started = false;
        }
        break;
    case dockingTask:
        if(iterator != positions.end())
        {
            ++currentTrial;
            ++iterator;
            ++angleIterator;
        }
        if(iterator != positions.end())
        {
            pos = iterator->first;
            loc = glm::toMat4(angleIterator->first);
            loc[3] = glm::vec4(pos,1);
            target->modelMatrix = loc;
            cursor->modelMatrix = glm::mat4();
        }
        if(iterator == positions.end())
        {
            target->setPosition(glm::vec3(5,0,-530));
            std::cout << "docking Experiment Finished";
            sessionTimer.stop();
            trialTimer.stop();
            started = false;
        }

        break;
    case rotationTask:
        if(angleIterator != angles.end())
        {
            ++currentTrial;
            ++angleIterator;
        }
        if(angleIterator != angles.end())
        {
            loc = glm::toMat4(angleIterator->first);
            target->modelMatrix = loc;
            cursor->modelMatrix = glm::mat4();
        }
        if(iterator == positions.end())
        {
            target->setPosition(glm::vec3(5,0,-530));
            std::cout << "rotation Experiment Finished";
            trialTimer.stop();
            sessionTimer.stop();
            started = false;
        }

        break;
    }
    plane->modelMatrix = cursor->modelMatrix;
    trialTime = trialTimer.elapsed();

    std::cout << currentTrial << std::endl;
}



void pho::ExpManager::randomizePositions()
{
    positions = originalPositions;
    std::srand(time(NULL));
    std::random_shuffle(positions.begin(),positions.end());
    angles = originalAngles;
    std::random_shuffle(angles.begin(),angles.end());

}

glm::vec3 pho::ExpManager::magnify(glm::vec3 &tomagnify, int by)
{
    return glm::vec3(tomagnify.x*by,tomagnify.y*by,tomagnify.z*by);
}


void pho::ExpManager::setTarget(pho::Cursor *value)
{
    target = value;
}

void pho::ExpManager::setCursor(pho::Cursor *value)
{
    cursor = value;
}

