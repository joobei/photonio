#pragma warning(disable: 4098)
#pragma warning(disable: 4819)

#include "uikun.h"
#include "boost/bind.hpp"
#include <functional>


int main()
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (!glfwInit()==GL_TRUE) {  std::cout << "glfw initialization failed";  return 1;  }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    //glfwWindowHint(GLFW_SAMPLES,8);

    GLFWwindow* mainWindow = glfwCreateWindow(pho::Engine::WINDOW_SIZE_X,pho::Engine::WINDOW_SIZE_Y,"Plane-Casting",NULL,NULL);
    if (!mainWindow)
    {
        std::cout << "GLFW Init Window Failed" << std::endl;
    }

    glfwMakeContextCurrent(mainWindow);
    std::cout << "OpenGL Version: " << glGetString( GL_VERSION ) << std::endl;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
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
