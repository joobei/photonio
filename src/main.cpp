#pragma warning(disable: 4098)
#pragma warning(disable: 4819)

#include "uikun.h"
#include "boost/bind.hpp"
#include <functional>

typedef void (*GLFWmousebuttonfun)(int,int);
void glfwMouseCallback(GLFWmousebuttonfun f){}
std::function<void (int,int)> mouseFunc;
void mouseFunkThunk(int x, int y)
{
    mouseFunc(x, y);
}

typedef void (*GLFWmouseposfun)(int,int);
void glfwMouseCallback2(GLFWmouseposfun f){}
std::function<void (int,int)> mouseFunc2;
void mouseFunkThunk2(int x, int y)
{
    mouseFunc2(x, y);
}

int main()
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (!glfwInit()==GL_TRUE) {  std::cout << "glfw initialization failed";  return 1;  }

    glfwOpenWindowHint(GLFW_VERSION_MAJOR,3);
    glfwOpenWindowHint(GLFW_VERSION_MINOR,3);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES,8);
	//glfwOpenWindowHint( GLFW_STEREO, GL_TRUE );

	if (!glfwOpenWindow(pho::Engine::WINDOW_SIZE_X,pho::Engine::WINDOW_SIZE_Y,0,0,0,0,0,0,GLFW_WINDOW))
    { std::cout << "GLFW Init WIndow Failed" << std::endl; }

    glfwSetWindowPos(pho::Engine::WINDOW_POS_X,pho::Engine::WINDOW_POS_X);

    glfwSetWindowTitle("Plane-Casting");
	glfwEnable( GLFW_MOUSE_CURSOR );

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

    pho::Engine *engine = new Engine();

    if (glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_PRESENT) && (glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_AXES) > 4))
    {
        log("Space Navigator Present");
        engine->joystick = GLFW_JOYSTICK_1;
    }
    else if (glfwGetJoystickParam(GLFW_JOYSTICK_2,GLFW_PRESENT) && (glfwGetJoystickParam(GLFW_JOYSTICK_2,GLFW_AXES) > 4))
    {
        log("Space Navigator Present");
        engine->joystick = GLFW_JOYSTICK_2;
    }
    else
    {
        log("Space Navigator not present");
    }

    mouseFunc = std::bind(&pho::Engine::mouseButtonCallback,engine,_1,_2);
    glfwSetMouseButtonCallback(&mouseFunkThunk);
    mouseFunc2 = std::bind(&pho::Engine::mouseMoveCallback,engine,_1,_2);
    glfwSetMousePosCallback(&mouseFunkThunk2);



    engine->go();

    delete engine;
    glfwTerminate();
    return 0;
}
