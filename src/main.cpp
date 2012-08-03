#include "photonio.h"
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
    { std::cout << "GLFW Init WIndow Failed" << std::endl;
    }

#if defined(_DEBUG)
	glfwSetWindowPos(400,0);
#endif
    glfwSetWindowTitle("KeiMote");


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong. 
      std::cout << "GLEW Error: " << glewGetErrorString(err) <<std::endl;

    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) <<std::endl;
	

    pho::Engine *engine = new Engine();

    mouseFunc = std::bind(&pho::Engine::mouseButtonCallback,engine,_1,_2);
    glfwSetMouseButtonCallback(&mouseFunkThunk);
    mouseFunc2 = std::bind(&pho::Engine::mouseMoveCallback,engine,_1,_2);
    glfwSetMousePosCallback(&mouseFunkThunk2);

    engine->go();

    glfwTerminate();
    return 0;
}
