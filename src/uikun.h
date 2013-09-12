/*  Photonio Graphics Engine
Copyright (C) 2011 Nicholas Katzakis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef PHOTONIO_H
#define PHOTONIO_H
#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"
#include <sstream>
#include "util.h"
#include <boost/static_assert.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/scoped_array.hpp>
#include "eventQueue.h"
#include "asio.h"
#include "spuc/generic/running_average.h"
//#include "arcball.h"
#include "asset.h"
#include "cursor.h"
#include "TUIO/TuioClient.h"
#include "TUIO/TuioListener.h"
#include "TUIO/TuioObject.h"
#include "TUIO/TuioCursor.h"
#include "TUIO/TuioPoint.h"
#include <cstdio>
#include <functional>
#include "shader.h"
#include "plane.h"
#include <GL/glfw.h>
#include "gli/gli.hpp"
#include "gli/gtx/gl_texture2d.hpp"
#include <btBulletCollisionCommon.h>
#include "experimentManager.h"

using namespace std;
using namespace TUIO;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#define LOGITECH_VENDOR_ID 0x46d

namespace pho {
    class Engine: public TuioListener {

	public:
		Engine();
		void checkEvents();
		void render();
		void recursive_render(const struct aiScene *sc, const struct aiNode* nd);
		void initResources();
		void go();
		bool computeRotationMatrix();
		void shutdown();

        static const int TOUCH_SCREEN_SIZE_X = 480;
        static const int TOUCH_SCREEN_SIZE_Y = 800;

        static const int WINDOW_SIZE_X = 1920;
        static const int WINDOW_SIZE_Y = 1080;

        static const int WINDOW_POS_X =  0;
        static const int WINDOW_POS_Y =  0;

        //static const int WINDOW_SIZE_X = 1280;
        //static const int WINDOW_SIZE_Y = 720;
        //static const int WINDOW_POS_X = 810;
        //static const int WINDOW_POS_Y = 30;

		void mouseButtonCallback(int x, int y);
		void mouseMoveCallback(int x, int y);

		void addTuioObject(TuioObject *tobj);
		void updateTuioObject(TuioObject *tobj); 
		void removeTuioObject(TuioObject *tobj);

		void addTuioCursor(TuioCursor *tcur);
		void updateTuioCursor(TuioCursor *tcur);
		void removeTuioCursor(TuioCursor *tcur);

		TuioClient* tuioClient;
        std::vector<glm::vec3> linestack;
        int spaceNavigator;
        int moveController;
	private:

        void ScreenPosToWorldRay(
            int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
            int screenWidth, int screenHeight,  // Window size, in pixels
            glm::mat4 ViewMatrix,               // Camera position and orientation
            glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
            glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
            glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
        );

		void checkUDP();
		void checkKeyboard();
		void checkSpaceNavigator();
        void checkMove();
        void initPhysics();
        void checkPhysics();
        bool rayTest(const float &normalizedX,const float &normalizedY, pho::Asset*& intersected);
        bool rayTestWorld(const glm::vec3 &origin,const glm::vec3 &direction, pho::Asset*& intersectedw);

		// map image filenames to textureIds
		// pointer to texture Array
		std::map<std::string, GLuint> textureIdMap;	
		bool LoadGLTextures(const aiScene* scene);

        AppState appState;
		RotateTechnique rotTechnique;
        SelectionTechnique selectionTechnique;
		Technique technique;

		GLenum error;

		mat4 orientation,calibration;
		mat3 axisChange;
		mat4 trackerMatrix;
		mat3 orientation3;
		vec3 acc,ma,gyro;
		bool calibrate;
		bool gyroData;
		
		glm::vec3 cameraPosition;
		glm::vec3 cameraDirection;

        //Shaders
        pho::Shader normalMap;
        pho::Shader noTextureShader;
        pho::Shader singleTexture;

		//Picking
        glm::mat4 InverseProjectionMatrix;
        glm::mat4 InverseViewMatrix;

		GLuint hitObject;
		bool objectHit,sphereHit;
		
		float rayLength;
		float rayLengthObject;
		bool restoreRay;
		glm::vec3 grabOffset;
		float grabbedDistance;
		glm::vec3 grabbedVector;
        glm::vec3 intersectionPoint;

        //scaled homer SEKI


        glm::vec3 initPosition;

        //for rotation
        glm::mat4 objectInitMatrix;
        glm::mat4 receiverInitMatrix;

        glm::mat4 objectprevModelMatrix;

        glm::mat4 polhemusMatrix;

        bool directRotationStarted = false;
        bool directTranslationStarted = false;
        bool first = false;


		glm::vec3 rayOrigin,rayDirection;

		EventQueue eventQueue;
		SPUC::running_average<float> accelerometerX,accelerometerY,accelerometerZ,magnetometerX,magnetometerY,magnetometerZ;

        sharedResources sr;

		//SmartPhone IO
		boost::asio::io_service ioservice;
        boost::asio::io_service serialioservice;
		boost::asio::io_service::work udpwork;
		boost::thread* netThread;
        boost::thread* serialThread;
		udp_server _udpserver;
        Minicom_client _serialserver;

        bool checkPolhemus(glm::mat4 &modelMatrix);

		boost::mutex ioMutex; //locks the message queue for thread access

        //assets
        pho::Asset cursor;
        pho::Asset s0;
        pho::Asset s1;
        pho::Asset s2;
        pho::Asset s3;
        pho::Plane plane;
        pho::Cursor pyramidCursor;
        pho::Cursor target;
        pho::Ray ray;
        pho::Asset floor;      
        pho::Asset* selectedAsset;
        pho::Asset* intersectedAsset;

        glm::vec3 red = glm::vec3(1,0,0);
        glm::vec3 green = glm::vec3(0,1,0);
        glm::vec3 blue = glm::vec3(0,0,1);
        glm::vec3 yellow = glm::vec3(1,1,0);

		GLuint rayVBO;
		short rayVerticesCount;
		
		//perspective matrix control
		float perspective;

		//TUIO input stuff
		void refresh(TuioTime frameTime);
		bool f1,f2;
		bool verbose;
		int f1id,f2id;
		float referenceAngle;
		glm::vec2 p1p,p2p,p1c,p2c;
		glm::vec2 p1t,p2t;
        glm::vec2 p1f; //first point of contact of 1st finger (to decide axis lock or not)
        float lastFingerDistance;
		bool both;
		bool consumed;

        //Flicking
        pho::flickManager flicker;
        boost::timer::cpu_timer flickTimer;

        //Timers
        boost::timer::cpu_timer doubleClick;
        boost::timer::cpu_timer keyboardTimer;

        boost::timer::cpu_times previousTime;
        boost::timer::cpu_times keyboardPreviousTime;
        bool keyPressOK=true;

        float pointerOpacity;
        bool pointerAvailable;

        //lighting
        pho::LightSource pointLight;


		//mouse 
		float prevMouseWheel;
		vec2 prevMousePos;
		bool prevMouseExists;

		std::ofstream errorLog;

		//time
		boost::posix_time::time_facet *tf;
		float deltat;

		//Textures
        std::string assetpath;

        //Shadowmap ****************************
        GLuint shadowFBO;
        void generateShadowFBO();
        void shadowMapRender();
        GLuint shadowMapLoc;
        GLuint baseImageLoc;


        //Physics
        //bool switchOnNextFrame=false;

        //Indirect Finger
        glm::vec2 touchPoint;
        GLuint pointVao;

        //EXPERIMENT:
        ExpManager experiment;
	};

}


#endif
