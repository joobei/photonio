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
#include <vector>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <sstream>
#include "util.h"
#include <boost/static_assert.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer.hpp> //for stopwatch and time output
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "eventQueue.h"
#include "asio.h"
#include "spuc/generic/running_average.h"
#include "arcball.h"
#include "assets.h"
#include "TUIO/TuioClient.h"
#include "TUIO/TuioListener.h"
#include "TUIO/TuioObject.h"
#include "TUIO/TuioCursor.h"
#include "TUIO/TuioPoint.h"
#include <cstdio>
#include <functional>
#include "wiimote.h"
#include "shader.h"
#include "mesh.h"


#include <GL/glfw.h>

using namespace std;
using namespace TUIO;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


namespace pho {

	enum InputState { //appInputState
		idle,
		translate,
		rotate
	};

	enum RotateTechnique { //rotTechnique
		singleAxis,
		pinch2,
		screenSpace,
		pinch,
		trackBall
	};

	enum AppMode {  //appmode
		planeCasting,
		rayCasting
	};

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

		static const int WINDOW_SIZE_X = 800;
		static const int WINDOW_SIZE_Y = 600;

		void mouseButtonCallback(int x, int y);
		void mouseMoveCallback(int x, int y);

		void addTuioObject(TuioObject *tobj);
		void updateTuioObject(TuioObject *tobj); 
		void removeTuioObject(TuioObject *tobj);

		void addTuioCursor(TuioCursor *tcur);
		void updateTuioCursor(TuioCursor *tcur);
		void removeTuioCursor(TuioCursor *tcur);

		TuioClient* tuioClient;

	private:
		void initSimpleGeometry();

		// map image filenames to textureIds
		// pointer to texture Array
		std::map<std::string, GLuint> textureIdMap;	
		bool LoadGLTextures(const aiScene* scene);

		InputState appInputState;
		RotateTechnique rotTechnique;
		AppMode appMode;


		GLenum error;

		mat4 orientation,calibration;
		mat3 axisChange;
		mat4 trackerMatrix;
		mat3 orientation3;
		mat4 projectionMatrix, viewMatrix;
		vec3 acc,ma,gyro;
		float arcBallPreviousPoint[2];
		bool calibrate;
		bool gyroData;
		
		glm::vec3 cameraPosition;
		glm::vec3 cameraDirection;

		//Shaders
        pho::Shader colorShader,offscreenShader;

		//Lights
		GLuint lightSource;
		pho::LightSource pointLight;

		//Picking
		GLuint hitObject;
		bool objectHit,sphereHit;

		GLuint pickProgram;
		GLuint tex; 
		GLuint rbo; 
		GLuint fbo;
		GLuint pbo_a,pbo_b;
		GLenum DrawBuffers[2];
		float rayLength;
		bool restoreRay;
		glm::vec3 grabOffset;
		float grabbedDistance;
		
		vec3 objectIntersectionPoint;	
		vec3 sphereIntersectionPoint;
		vec3 sphereIntersectionNormal;
		float sphereIntersectionDistance; //not really needed

		EventQueue eventQueue;
		SPUC::running_average<float> accelerometerX,accelerometerY,accelerometerZ,magnetometerX,magnetometerY,magnetometerZ;

		//SmartPhone IO
		boost::asio::io_service ioservice;
		boost::thread* netThread;
		udp_server _udpserver;

		//Polhemus IO
		boost::asio::io_service serialioservice;
		boost::thread* serialThread;
		Minicom_client _serialserver;

		boost::mutex ioMutex; //locks the message queue for thread access

		//assets
        pho::Mesh target;
        pho::Mesh cursor;
        pho::Mesh plane;
        pho::Mesh ray;
		pho::Mesh point;

		//raycasting test
		int count;
		
		//perspective matrix control
		float perspective;

		//TUIO input stuff
		void refresh(TuioTime frameTime);
		glm::vec2 xyOrigin;
		glm::vec3 tempOrigin;
		TUIO::TuioCursor* trackedCursor;
		int trackedCursorId;
		glm::vec2 trackedCursorPrevPoint;
		bool f1,f2;
		bool verbose;
		int f1id,f2id;
		glm::vec2 f1prev,f2prev;
		glm::vec2 f1speed,f2speed;
		float referenceAngle;
		glm::vec2 p1p,p2p,p1c,p2c;
		glm::vec2 p1t,p2t;
		bool both;

		//**************NEW ALGORITHM v2*******************

		glm::vec2 f1c,f1p,f2c,f2p;


		//*************************************************


		//Wii-Mote Stuff
		pho::WiiButtonState wiiButton;
		wiimote remote;
		bool wii;


		//mouse wheel
		int prevMouseWheel;

		std::ofstream errorLog;

		//time
		boost::posix_time::time_facet *tf;
		float deltat;
	};

}


#endif
