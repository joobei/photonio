#include "experimentManager.h"
using namespace std;
using namespace boost;

pho::ExpManager::ExpManager()
{
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

    originalPositions.push_back(std::make_pair(p0,1));
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
    originalPositions.push_back(std::make_pair(p11,12));

    short multiplier = 2;

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

    multiplier = 4;

    originalPositions.push_back(std::make_pair(magnify(p0,multiplier),24+1));
    originalPositions.push_back(std::make_pair(magnify(p1,multiplier),24+2));
    originalPositions.push_back(std::make_pair(magnify(p2,multiplier),24+3));
    originalPositions.push_back(std::make_pair(magnify(p3,multiplier),24+4));
    originalPositions.push_back(std::make_pair(magnify(p4,multiplier),24+5));
    originalPositions.push_back(std::make_pair(magnify(p5,multiplier),24+6));
    originalPositions.push_back(std::make_pair(magnify(p6,multiplier),24+7));
    originalPositions.push_back(std::make_pair(magnify(p7,multiplier),24+8));
    originalPositions.push_back(std::make_pair(magnify(p8,multiplier),24+9));
    originalPositions.push_back(std::make_pair(magnify(p9,multiplier),24+10));
    originalPositions.push_back(std::make_pair(magnify(p10,multiplier),24+11));
    originalPositions.push_back(std::make_pair(magnify(p11,multiplier),24+12));

    for (auto i=0;i<3;++i) {
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,0,0))),1));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,0,1))),2));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,1,0))),3));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(0,1,1))),4));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,0,0))),5));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,0,1))),6));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,1,0))),7));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(90.f,glm::vec3(1,1,1))),8));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,0,0))),9));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,0,1))),10));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(0,1,0))),11));
        angles.push_back(std::make_pair(glm::normalize(glm::angleAxis(180.f,glm::vec3(1,1,0))),12));
    }

    currentExperiment = practice;
    noOfTrials = originalPositions.size();

    bigLogFile.open ("bigFile.txt");
    smallLogFile.open ("smallFile.txt");

    positions = originalPositions;
    angles = originalAngles;

    iterator = positions.begin();
    angleIterator = angles.begin();

    sessionTimer.stop();
    trialTimer.stop();
    experimentTimer.stop();
}

void pho::ExpManager::setRay(pho::Ray *value)
{
    ray = value;
}

void pho::ExpManager::reset()
{
    currentTrial = 1;
    sessionTimer.stop();
    trialTimer.stop();
    started = false;
    iterator = positions.begin();
    angleIterator = angles.begin();
    randomizePositions();
}

void pho::ExpManager::start()
{
    glm::vec3 pos;
    glm::mat4 loc;

    sessionTimer.start();
    experimentTimer.start();

    currentTrial = 1;
    switch (currentExperiment) {
    case dockingTask:
        iterator = positions.begin();
        pos = positions[currentTrial].first;
        loc = glm::toMat4(angles[currentTrial].first);
        loc[3] = glm::vec4(pos,1);
        target->modelMatrix = loc;
        cursor->modelMatrix = glm::mat4();
        break;
    case rotationTask:
        break;

    }
}

void pho::ExpManager::setUser(const std::string &value)
{
    user = value;
}

void pho::ExpManager::closeFiles()
{
    bigLogFile.close();
    smallLogFile.close();
}

void pho::ExpManager::log()
{

    boost::timer::cpu_times const elapsed_times(trialTimer.elapsed());
    double difference = elapsed_times.wall-trialTime.wall;

    boost::timer::cpu_times const elapsed_session(sessionTimer.elapsed());
    double differenceSession = elapsed_session.wall-sessionTime.wall;

    boost::timer::cpu_times const elapsed_experiment(experimentTimer.elapsed());
    double differenceExperiment = elapsed_experiment.wall-experimentTime.wall;

    glm::quat cur = glm::toQuat(cursor->modelMatrix);
    glm::quat tar = glm::toQuat(target->modelMatrix);

    started = false;

    if (started) {

        bigLogFile << user << ';';
        bigLogFile << currentFrame++ << ';';
        bigLogFile << currentTrial << ';';
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

        touchLogFile << finger1.exists << ';';
        touchLogFile << finger1.id << ';';
        touchLogFile << finger1.x << ';';
        touchLogFile << finger1.y << ';';
        touchLogFile << finger2.exists << ';';
        touchLogFile << finger2.id << ';';
        touchLogFile << finger2.x << ';';
        touchLogFile << finger2.y << ';';

        bigLogFile << pedal << ';';
        bigLogFile << difference/1000000.0f << ';';
        bigLogFile << differenceSession/1000000.0f << ';';
        bigLogFile << differenceExperiment/1000000.0f << ';';
        //add APP STATES
        //flick states
        bigLogFile << '\n';
    }
}


bool pho::ExpManager::advance()
{
    float x,y,z;
    glm::vec3 pos;
    glm::mat4 loc;

    trialTime = trialTimer.elapsed();

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
            currentTrial++;
            ++iterator;
            ++angleIterator;
        }
        if(iterator != positions.end())
        {
           target->setPosition(iterator->first);
            //loc = glm::toMat4(angleIterator->first);
            //loc[3] = glm::vec4(pos,1);
            //target->modelMatrix = loc;
            cursor->modelMatrix = glm::mat4();
        }
        if(iterator == positions.end())
        {
            target->setPosition(glm::vec3(5,0,-530));
            std::cout << "Experiment Finished";
        }


    case dockingTask:

        if(iterator != positions.end())
        {
            currentTrial++;
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
            std::cout << "Experiment Finished";
        }

        break;
    case rotationTask:
        if (!started) {
            started = !started;
        }
        else {
            currentTrial++;
            ++angleIterator;
            if(iterator == positions.end())
            {
                target->setPosition(glm::vec3(5,0,-530));
                std::cout << "Experiment Finished";
            }
        }
        break;
    }
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
