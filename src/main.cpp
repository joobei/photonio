#pragma warning(disable: 4098)
#pragma warning(disable: 4819)

#include "uikun.h"
#include "boost/bind.hpp"
#include <functional>


int main()
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (!glfwInit()==GL_TRUE) {  std::cout << "glfw initialization failed";  return 1;  }

    glfwWindowHint(GLFW_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_VERSION_MINOR,3);
    glfwWindowHint(GLFW_SAMPLES,8);
    //glfwWindowHint( GLFW_STEREO, GL_TRUE );
    GLFWwindow* mainWindow = glfwCreateWindow(pho::Engine::WINDOW_SIZE_X,pho::Engine::WINDOW_SIZE_Y,"Plane-Casting",NULL,NULL);
    if (!mainWindow)
    { std::cout << "GLFW Init WIndow Failed" << std::endl; }

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong. 
      std::cout << "GLEW Error: " << glewGetErrorString(err) <<std::endl;

    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) <<std::endl;

    //int iu = 0;
    //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&iu);
    //std::cout << "Texture units " << iu <<std::endl;

    pho::Engine *engine = new Engine(mainWindow);

    /*if (glfwGetJoystickPresent(0) {
        std::cout << "joystick present" << std::endl;

		
        std::cout << "joystick axes" << glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_AXES) << std::endl;
	}
    else { std::cout << "joystick not present" << std::endl;}*/

    engine->go();

    delete engine;
    glfwTerminate();
    return 0;
}
