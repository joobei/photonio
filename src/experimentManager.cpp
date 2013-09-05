#include "experimentManager.h"

pho::ExpManager::ExpManager()
{
    originalPositions.push_back(glm::vec3(0.000000,-1.000000,0.000000));
    originalPositions.push_back(glm::vec3(0.723600,-0.447215 ,0.525720));
    originalPositions.push_back(glm::vec3(-0.276385, -0.447215, 0.850640));
    originalPositions.push_back(glm::vec3(-0.894425, -0.447215, 0.000000));
    originalPositions.push_back(glm::vec3(-0.276385, -0.447215, -0.850640));
    originalPositions.push_back(glm::vec3(0.723600, -0.447215, -0.525720));
    originalPositions.push_back(glm::vec3(0.276385, 0.447215 ,0.850640));
    originalPositions.push_back(glm::vec3(-0.723600, 0.447215, 0.525720));
    originalPositions.push_back(glm::vec3(-0.723600, 0.447215, -0.525720));
    originalPositions.push_back(glm::vec3(0.276385, 0.447215, -0.850640));
    originalPositions.push_back(glm::vec3(0.894425, 0.447215, 0.000000));
    originalPositions.push_back(glm::vec3(0.000000, 1.000000, 0.000000));

    currentExperiment = practice;

    noOfTrials = originalPositions.size();

}

void pho::ExpManager::setRay(pho::Ray *value)
{
    ray = value;
}

void pho::ExpManager::setUser(const std::string &value)
{
    user = value;
}


bool pho::ExpManager::advance()
{
    float x,y,z;
    if(currentTrial != noOfTrials)
    {
        switch (currentExperiment) {
        case practice:
            x = -10 + (float)rand()/((float)RAND_MAX/(10-(-10)));
            y = -10 + (float)rand()/((float)RAND_MAX/(10-(-10)));
            z = -10 + (float)rand()/((float)RAND_MAX/(0.1-(-0.1)));
            target->setPosition(glm::vec3(x,y,z));
            break;
        case dockingTask:
            currentTrial++;
            target->setPosition(positions[currentTrial]);
            break;
        case rotationTask:
            currentTrial++;
            target->setPosition(positions[currentTrial]);
            break;
        }
    }
    else {
        std::cout << "Experiment Finished" << std::endl;
        currentTrial = 0;
        //endExperiment();
    }
}



void pho::ExpManager::randomizePositions()
{
    positions = originalPositions;
    std::random_shuffle(positions.begin(),positions.end());

}


void pho::ExpManager::setTarget(pho::Cursor *value)
{
    target = value;
}



void pho::ExpManager::setCursor(pho::Cursor *value)
{
    cursor = value;
}


void pho::ExpManager::setSphericalCursor(pho::Asset *value)
{
    sphericalCursor = value;
}



void pho::ExpManager::setWandPosition(glm::vec3 *value)
{
    wandPosition = value;
}



void pho::ExpManager::setRightWristRotation(glm::vec4 *value)
{
    rightWristRotation = value;
}



void pho::ExpManager::setRightWristPosition(glm::vec3 *value)
{
    rightWristPosition = value;
}



void pho::ExpManager::setLeftWristRotation(glm::vec4 *value)
{
    leftWristRotation = value;
}



void pho::ExpManager::setLeftWristPosition(glm::vec3 *value)
{
    leftWristPosition = value;
}
