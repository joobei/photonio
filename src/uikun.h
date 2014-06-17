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

#include <OpenGL/gl3.h>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#define GLM_FORCE_RADIANS
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
#include "TUIO/TuioClient.h"
#include "TUIO/TuioListener.h"
#include "TUIO/TuioObject.h"
#include "TUIO/TuioCursor.h"
#include "TUIO/TuioPoint.h"
#include <cstdio>
#include <functional>
#include "shader.h"
#include "plane.h"
#include <GLFW/glfw3.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


using namespace std;
using namespace TUIO;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

namespace pho {
    class Engine: public TuioListener {

	public:
        Engine(GLFWwindow* window);
		void checkEvents();
		void render();
		void recursive_render(const struct aiScene *sc, const struct aiNode* nd);
		void initResources();
		void go();
		bool computeRotationMatrix();
		void shutdown();


        static const int TOUCH_SCREEN_SIZE_X = 480;
		static const int TOUCH_SCREEN_SIZE_Y = 800;

        //static const int WINDOW_SIZE_X = 1920;
        //static const int WINDOW_SIZE_Y = 1080;
        static const int WINDOW_SIZE_X = 1280;
        static const int WINDOW_SIZE_Y = 720;

		void mouseButtonCallback(int x, int y);
		void mouseMoveCallback(int x, int y);

		void addTuioObject(TuioObject *tobj);
		void updateTuioObject(TuioObject *tobj); 
		void removeTuioObject(TuioObject *tobj);

		void addTuioCursor(TuioCursor *tcur);
		void updateTuioCursor(TuioCursor *tcur);
		void removeTuioCursor(TuioCursor *tcur);

        btVector3 getRayTo(glm::vec2 xy);
		TuioClient* tuioClient;
        std::vector<glm::vec3> linestack;

        //util
        glm::mat4 convertBulletTransformToGLM(const btTransform& transform);
        bool JoystickPresent;
	private:
		void checkUDP();
		void checkKeyboard();
		void checkSpaceNavigator();
        bool navmode = false;
        void initPhysics();
        void checkPhysics();
        bool rayTest(const float &normalizedX,const float &normalizedY, pho::Asset*& intersected);
        bool rayTestWorld(const glm::vec3 &origin,const glm::vec3 &direction, pho::Asset*& intersected);

		// map image filenames to textureIds
		// pointer to texture Array
		std::map<std::string, GLuint> textureIdMap;	
		bool LoadGLTextures(const aiScene* scene);

        AppState appState;
		RotateTechnique rotTechnique;
        SelectionTechnique selectionTechnique;
        bool bump=false; //*******************

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
        bool doubleClickPerformed =false;
		GLuint hitObject;
		bool objectHit,sphereHit;
		
		float rayLength;
		float rayLengthObject;
		bool restoreRay;
		glm::vec3 grabOffset;
		float grabbedDistance;
		glm::vec3 grabbedVector;

		vec3 objectIntersectionPoint;	
		vec3 sphereIntersectionPoint;
		vec3 sphereIntersectionNormal;
		float sphereIntersectionDistance; //not really needed

		//arcball stuff
		int last_mx,last_my,cur_mx,cur_my;
		bool mouseMove;
		glm::vec3 get_arcball_vector(glm::vec3 sphereOrigin, float radius,int x, int y);
		glm::vec3 rayOrigin,rayDirection;
		//arcball
		bool startDrag(const vec3& rayDirection, const vec3& rayOrigin);
		void Drag(const vec3& raydir, const vec3& rayOrigin, glm::mat4 viewMatrix);
		glm::vec3 previousVector;

		EventQueue eventQueue;
		SPUC::running_average<float> accelerometerX,accelerometerY,accelerometerZ,magnetometerX,magnetometerY,magnetometerZ;

        sharedResources sr;

		//SmartPhone IO
		boost::asio::io_service ioservice;
        //boost::asio::io_service serialioservice;
		boost::asio::io_service::work udpwork;
		boost::thread* netThread;
        //boost::thread* serialThread;
		udp_server _udpserver;
        //Minicom_client _serialserver;

        bool checkPolhemus(glm::mat4 &modelMatrix);

		boost::mutex ioMutex; //locks the message queue for thread access

        //assets
        pho::Asset cursor;
        pho::Asset heart;
        pho::Plane plane;
        pho::Asset ray;
        pho::Asset floor;
        pho::Asset* selectedAsset;
        pho::Asset* intersectedAsset;
        std::vector<pho::Asset> boxes;

        btGhostObject* coCursor;
        //input
        bool inputStarted;

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

        bool pointerOpacity;
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

        bool switchOnNextFrame=false;

        //Indirect Finger
        glm::vec2 touchPoint;
        GLuint pointVao;

        GLFWwindow* mainWindow;
    };

}


#endif
