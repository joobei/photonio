#pragma warning(disable: 4819)
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

#include "photonio.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <boost/math/special_functions/round.hpp>
#include "data.pb.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

Engine::Engine():
calibrate(false),
	eventQueue(),
	appInputState(idle),
	_udpserver(ioservice,&eventQueue,&ioMutex),
#if defined(_DEBUG)
	_serialserver(serialioservice,115200,"COM5",&eventQueue,&ioMutex),
#endif	
	wii(false)
{
#define SIZE 30                     //Size of the moving average filter
	accelerometerX.set_size(SIZE);  //Around 30 is good performance without gyro
	accelerometerY.set_size(SIZE);
	accelerometerZ.set_size(SIZE);
	magnetometerX.set_size(SIZE);
	magnetometerY.set_size(SIZE);
	magnetometerZ.set_size(SIZE);

	tuioClient = new TuioClient(3333);
	tuioClient->addTuioListener(this);
	tuioClient->connect();

	if (!tuioClient->isConnected()) {
		std::cout << "tuio client connection failed" << std::endl;
	}

	verbose = false;

	//Protobuf custom protocol listener
	netThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));
#if defined(_DEBUG)
	//Polhemus
	serialThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &serialioservice));
#endif	
	count=0;

	wii=remote.Connect(wiimote::FIRST_AVAILABLE);

	if (wii) { 	remote.SetLEDs(0x01); }
	else { errorLog << "WiiRemote Could not Connect \n"; }


	appInputState = idle; 
	appMode = planeCasting;
	rotTechnique = screenSpace;

	grabbing=false;

	errorLog.open("error.log",std::ios_base::app);

	prevMouseWheel = 0;
	gyroData = false;

	axisChange[0][0] = 1; axisChange[0][1] =  0;  axisChange[0][2] =  0;
	axisChange[1][0] = 0; axisChange[1][1] =  0;  axisChange[1][2] =  1;
	axisChange[2][0] = 0; axisChange[2][1] =  -1;  axisChange[2][2] = 0;

	
}

void Engine::initResources() {

	//*********** ASSIMP LOADING CODE****************
	
	glGenBuffers(1,&matricesUniBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
	glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc, matricesUniBuffer, 0, MatricesUniBufferSize);

	// Create an instance of the Importer class
	Assimp::Importer importer;

	std::string pFile;
	//pFile = "assets/normalheart.3DS";
	pFile = "assets/HumanHeart.obj";
	//pFile = "assets/california.3ds";
	//pFile = "assets/skull.3ds";
	

	//check if file exists
	std::ifstream fin(pFile.c_str());
	if(!fin.fail()) {
		fin.close();
	}
	else{
		std::cout << "Couldn't open file:"  << pFile << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
	}

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	scene = importer.ReadFile( pFile,
		aiProcess_CalcTangentSpace       |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType );

	scene = importer.GetOrphanedScene();

	LoadGLTextures(scene);

	// If the import failed, report it
	if( !scene) {
		std::cout <<  importer.GetErrorString() << std::endl;
	}
	else { 
		std::cout << "Import of geometry succeeded." << std::endl; 
	}


	//loop through meshes
	pho::MyMesh aMesh;
	pho::MyMaterial aMat;
	GLuint buffer;

	std::cout << "Number of meshes : " << scene->mNumMeshes << std::endl;

	// For each mesh
	for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[n];

		// create array with faces
		// have to convert from Assimp format to array
		unsigned int *faceArray;
		faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];

			memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(float));
			faceIndex += 3;
		}
		aMesh.numFaces = scene->mMeshes[n]->mNumFaces;

		// generate Vertex Array for mesh
		glGenVertexArrays(1,&(aMesh.vao));
		glBindVertexArray(aMesh.vao);

		// buffer for faces
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);


		// buffer for vertex positions
		if (mesh->HasPositions()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(vertexLoc);
			glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// buffer for vertex normals
		if (mesh->HasNormals()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(normalLoc);
			glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

			//for (int i=0;i<100;i++) {
			//	std::cout << mesh->mNormals[i].x << " "; 
			//}
			//std::cout << '\n';
		}
		

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
				texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

			}
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(texCoordLoc);
			glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		// create material uniform buffer
		struct aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];

		aiString texPath;	//contains filename of texture

		if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
				//bind texture
				unsigned int texId = textureIdMap[texPath.data];
				aMesh.texIndex = texId;
				aMat.texCount = 1;
			}
		else
			aMat.texCount = 0;

		float c[4];
		set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
		aiColor4D diffuse;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			color4_to_float4(&diffuse, c);
		memcpy(aMat.diffuse, c, sizeof(c));

		set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
		aiColor4D ambient;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
			color4_to_float4(&ambient, c);
		memcpy(aMat.ambient, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D specular;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
			color4_to_float4(&specular, c);
		memcpy(aMat.specular, c, sizeof(c));

		set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D emission;
		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
			color4_to_float4(&emission, c);
		memcpy(aMat.emissive, c, sizeof(c));

		float shininess = 0.0;
		unsigned int max;
		aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
		aMat.shininess = shininess;

		glGenBuffers(1,&(aMesh.uniformBlockIndex));
		glBindBuffer(GL_UNIFORM_BUFFER,aMesh.uniformBlockIndex);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void *)(&aMat), GL_STATIC_DRAW);
		
		myMeshes.push_back(aMesh);
		std::cout << "Added mesh :" << n << std::endl;
		std::cout << "Number of faces :" << aMesh.numFaces << std::endl;
		
	}

	//*********** END   ASSIMP LOADING CODE****************/

	pointLight.color = glm::vec4(1.0,1.0,1.0,1.0);
	//pointLight.direction  = glm::vec3(-0.5,-0.5,-0.5);
	pointLight.direction  = glm::vec3(-1.0,0.0,0.0);
	pointLight.position = glm::vec3(0,150,150);

	glGenBuffers(1,&(pointLight.uniformBlockIndex)); //generate buffer and store it's location in pointLight's member variable
	glBindBuffer(GL_UNIFORM_BUFFER,pointLight.uniformBlockIndex); 
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSource), (void *)(&pointLight), GL_STATIC_DRAW);

	//Prepare our shaders
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,readTextFile("shaders/shader.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER,readTextFile("shaders/shader.frag")));
	colorShader = CreateProgram(shaderList);
	colorShaderPvm = glGetUniformLocation(colorShader,"pvm");

	shaderList.clear();

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,readTextFile("shaders/offscreen.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER,readTextFile("shaders/offscreen.frag")));
	flatShader = CreateProgram(shaderList);
	flatShaderPvm = glGetUniformLocation(flatShader,"pvm");
	flatShaderColor = glGetUniformLocation(flatShader,"baseColor");
	
	shaderList.clear();

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,readTextFile("shaders/texader.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER,readTextFile("shaders/texader.frag")));
	textureShader = CreateProgram(shaderList);
	textureShaderPvm = glGetUniformLocation(textureShader,"pvm");
	textureShaderTexture = glGetUniformLocation(textureShader,"texturex");

	shaderList.clear();

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER,readTextFile("shaders/dirlightdiffambpix.vert")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER,readTextFile("shaders/dirlightdiffambpix.frag")));
	dirLight = CreateProgram(shaderList);
	dirLightTexUnit = glGetUniformLocation(dirLight,"texUnit");
	dirLightCamera = glGetUniformLocation(dirLight,"cameraPosition");
	dirLightPVM = glGetUniformLocation(dirLight,"pvm");
	dirLightNM = glGetUniformLocation(dirLight,"nm"); //normal matrix

	glUniformBlockBinding(dirLight, glGetUniformBlockIndex(dirLight,"Material"), materialUniLoc); 
	glUniformBlockBinding(dirLight, glGetUniformBlockIndex(dirLight,"Light"), lightUniLoc); 


	//Calculate the matrices
	projectionMatrix = glm::perspective(45.0f, (float)WINDOW_SIZE_X/(float)WINDOW_SIZE_Y,0.1f,1000.0f); //create perspective matrix
	//projectionMatrix = glm::mat4();

	cameraPosition = glm::vec3(0,0,-25); //translate camera back (i.e. world forward)

	viewMatrix = mat4();
	viewMatrix = glm::translate(viewMatrix,cameraPosition); 

	initSimpleGeometry();

	generate_frame_buffer_texture();
	generate_pixel_buffer_objects();

	//glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LEQUAL);
	//glDepthRange(-1000.0f, 1000.0f);

	CALL_GL(glLineWidth(3.5));

	//heart selected by default
	selectedObject = &heart;
	selectedObject->vaoId = aMesh.vao;

	restoreRay=false;
}

//checks event queue for events
//and co nsumes them all
void Engine::checkEvents() {

#define FACTOR 0.1
	if (glfwGetKey(GLFW_KEY_DOWN)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,-FACTOR));
	}
	if (glfwGetKey(GLFW_KEY_UP)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,FACTOR));
	}
	if (glfwGetKey(GLFW_KEY_LEFT)) {
		viewMatrix = glm::translate(viewMatrix, vec3(FACTOR,0,0));
	}
	if (glfwGetKey(GLFW_KEY_RIGHT)) {
		viewMatrix = glm::translate(viewMatrix, vec3(-FACTOR,0,0));
	}
	if (glfwGetKey(GLFW_KEY_PAGEUP)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,-FACTOR,0))*glm::lookAt(cameraPosition,glm::vec3(selectedObject->modelMatrix[0][3],selectedObject->modelMatrix[1][3],selectedObject->modelMatrix[2][3]),glm::vec3(0,1,0));
		
	}
	if (glfwGetKey(GLFW_KEY_PAGEDOWN)) {
		viewMatrix = glm::translate(viewMatrix, vec3(0,FACTOR,0));
	}
	if (glfwGetKey(GLFW_KEY_HOME)) {
		cameraDirection = glm::rotate(cameraDirection, (float)0.001, vec3(0,1,0));
		viewMatrix = glm::lookAt(cameraPosition,cameraPosition+cameraDirection,glm::vec3(0,1,0));
		
	}
	if (glfwGetKey(GLFW_KEY_END)) {
	}
	if (glfwGetKey(GLFW_KEY_SPACE)) {
		plane.modelMatrix = glm::mat4();
		selectedObject->modelMatrix = glm::mat4();
		viewMatrix = glm::translate(glm::mat4(),cameraPosition);
	}
	if (glfwGetKey('1') == GLFW_PRESS) {
		rotTechnique = screenSpace;
		std::cout << "Screen Space Rotation" << '\n';
	}
	if (glfwGetKey('2') == GLFW_PRESS) {
		rotTechnique = singleAxis;
		std::cout << "Single Axis Rotation" << '\n';
	}
	if (glfwGetKey('3') == GLFW_PRESS) {
		rotTechnique = trackBall;
		std::cout << "Virtual Trackball Rotation" << '\n';
	}
	int wheel = glfwGetMouseWheel();
	if (wheel != prevMouseWheel) {
		int amount = wheel - prevMouseWheel;
		viewMatrix = glm::translate(viewMatrix, vec3(0,0,amount));
		prevMouseWheel = wheel;
	}

	//UDP queue
	boost::mutex::scoped_lock lock(ioMutex);
	while (!eventQueue.isEmpty()) {
		//assign event to local temp var
		keimote::PhoneEvent tempEvent;
		tempEvent = eventQueue.pop();

		//check event to see what type it is
		switch (tempEvent.type()) {
		case keimote::ACCEL:
			accelerometerX.update(tempEvent.x());
			accelerometerY.update(tempEvent.y());
			accelerometerZ.update(tempEvent.z());

			acc.x = accelerometerX.get_result();
			acc.y = accelerometerY.get_result();
			acc.z = accelerometerZ.get_result();
			gyroData = false;
			break;
		case keimote::MAG:
			magnetometerX.update(tempEvent.x());
			magnetometerY.update(tempEvent.y());
			magnetometerZ.update(tempEvent.z());

			ma.x = magnetometerX.get_result();
			ma.y = magnetometerY.get_result();
			ma.z = magnetometerZ.get_result();
			gyroData = false;
			break;
		case keimote::GYRO:
			plane.modelMatrix[0][0] = tempEvent.m11(); plane.modelMatrix[0][1] = tempEvent.m21(); plane.modelMatrix[2][0] = tempEvent.m31();
			plane.modelMatrix[1][0] = tempEvent.m12(); plane.modelMatrix[1][1] = tempEvent.m22(); plane.modelMatrix[2][1] = tempEvent.m32();
			plane.modelMatrix[2][0] = tempEvent.m13(); plane.modelMatrix[2][1] = tempEvent.m23(); plane.modelMatrix[2][2] = tempEvent.m33();
			gyroData = true;
			break;
		case keimote::BUTTON:
			switch (tempEvent.buttontype()) {
			case 1:
				calibrate = true;
				break;
			case 2:
				if (appInputState != rotate) {
					printf("idle-->rotate");
					appInputState = rotate; }
				else
					{ appInputState = idle; 
					printf("rotate-->idle");
				}

				break;
			case 3:
				break;
			default:
				calibrate = true;
				break;
			}

		};
		if (computeRotationMatrix() && (!gyroData)) {
			plane.modelMatrix[0][0] = orientation[0][0]; plane.modelMatrix[0][1] = orientation[0][1]; plane.modelMatrix[0][2] = orientation[0][2]; 
			plane.modelMatrix[1][0] = orientation[1][0]; plane.modelMatrix[1][1] = orientation[1][1]; plane.modelMatrix[1][2] = orientation[1][2]; 
			plane.modelMatrix[2][0] = orientation[2][0]; plane.modelMatrix[2][1] = orientation[2][1]; plane.modelMatrix[2][2] = orientation[2][2]; 
		}
	}


	//SERIAL Queue
	while(!eventQueue.isSerialEmpty()) {
		boost::array<float,7> temp = eventQueue.serialPop();
		glm::vec3 position;
		glm::quat orientation;
		glm::mat4 transform;

		position = glm::vec3(temp[0],temp[1],temp[2]);

#if defined(_DEBUG)
		position.x-=25;
		position.y+=30;
		position.z-=20;
#else 
		//position.x+=0;
		position.y-=5;
		position.z-=1;

#endif

		//std::cout << "Polhemus x: " << position.x << '\t' << "y: " << position.y << '\t' << "z: " << position.z << '\n';

		orientation.w = temp[3];
		orientation.x = temp[4];
		orientation.y = temp[5];
		orientation.z = temp[6];

		glm::mat4 rot = glm::toMat4(orientation);
		glm::mat4 trans = glm::translate(glm::mat4(),position);	
		transform = glm::toMat4(glm::angleAxis(180.0f,glm::vec3(0,1,0)))*transform;
		transform = glm::toMat4(glm::angleAxis(-90.0f,glm::vec3(0,0,1)))*transform;


		transform = rot*transform;

		transform= trans*transform;
		
		ray.modelMatrix = transform;

		

		//std::cout << "x : " << transform[3][0] << "\ty : " << transform[3][1] << "\tz : " << transform[3][2] << '\n';
	}
	lock.unlock();

	//Ray length calculation
	if (!grabbing && picked !=0) {
		rayLength = -glm::distance(
			glm::vec3(ray.getPosition()),
			glm::vec3(selectedObject->getPosition())
			);
	}

	//if the connection to the wii-mote was successful
	if (wii) {
		remote.RefreshState();

		switch(appMode)  {
		case rayCasting:
			if (appInputState == idle && remote.Button.B()) {   
				appInputState = translate;
				grabbedDistance = rayLength;

				mat4 newMat = glm::translate(ray.modelMatrix,glm::vec3(0,0,grabbedDistance));

				selectedObject->modelMatrix[3][0] = newMat[3][0];
				selectedObject->modelMatrix[3][1] = newMat[3][1];
				selectedObject->modelMatrix[3][2] = newMat[3][2];
				std::cout << "translate" << '\n';
			}
			if (appInputState == translate && remote.Button.B()) {
				mat4 newMat = glm::translate(ray.modelMatrix,glm::vec3(0,0,grabbedDistance));
				selectedObject->modelMatrix[3][0] = newMat[3][0];
				selectedObject->modelMatrix[3][1] = newMat[3][1];
				selectedObject->modelMatrix[3][2] = newMat[3][2];
			}
			if (appInputState == translate && remote.Button.Down() && grabbedDistance < 0) {
				grabbedDistance+=0.5;
			}
			if (appInputState == translate && remote.Button.Up()) {
				grabbedDistance-=0.5;
			}
			if (appInputState == translate && !remote.Button.B()) {
				appInputState = idle;
				std::cout << "idle" << '\n';
				break;
			}
			if (appInputState == idle && remote.Button.A()) {
				appInputState = rotate;
				rotTechnique = trackBall;

				std::cout << "trackball" << '\n';

				int xx,yy;
				
				glfwGetMousePos(&xx,&yy);
				arcBallPreviousPoint[0] = xx*1.0f;
				arcBallPreviousPoint[1] = yy*1.0f;
				tempOrigin = glm::vec3(selectedObject->modelMatrix[3][0],selectedObject->modelMatrix[3][1],selectedObject->modelMatrix[3][2]);
				break;
			}
//			if (appInputState == rotate && rotTechnique = trackBall && remote.Button.A()) {
				//trackball rotate
//				break;
//			}
		default:
			break;
		}
	}

}

void Engine::recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{
	CALL_GL(glUniformMatrix4fv(dirLightPVM, 1, GL_FALSE, glm::value_ptr(projectionMatrix*viewMatrix*selectedObject->modelMatrix)));
	CALL_GL(glUniformMatrix4fv(dirLightNM, 1, GL_FALSE, glm::value_ptr(viewMatrix*selectedObject->modelMatrix)));
	
	CALL_GL(glBindBufferRange(GL_UNIFORM_BUFFER, lightUniLoc, pointLight.uniformBlockIndex, 0, sizeof(struct pho::LightSource)));
	// draw all meshes assigned to this node
	for (unsigned int n=0; n < nd->mNumMeshes; ++n){
		// bind material uniform
		CALL_GL(glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct pho::MyMaterial)));
		// bind texture
		CALL_GL(glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex));
		// bind VAO
		CALL_GL(glBindVertexArray(myMeshes[nd->mMeshes[n]].vao));
		// draw
		CALL_GL(glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0));
	}

	// draw all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n){
		recursive_render(sc, nd->mChildren[n]);
	}
	
	//glEC("recursive");
}

void Engine::render() {
	CALL_GL(glClearColor(1,1,1,1));
	CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	
	//render off-screen for picking
	if (appMode == rayCasting) {
		picked = picking(scene,scene->mRootNode);

		if(picked !=0) {

			//render selection red border first with the flat color shader
			CALL_GL(glUseProgram(flatShader));
			CALL_GL(glUniform4f(flatShaderColor, 1.0f, 0.0f ,0.0f, 0.6f)); 	
			pvm = projectionMatrix*viewMatrix*glm::scale(selectedObject->modelMatrix,glm::vec3(1.1f,1.1f,1.1f));
			CALL_GL(glUniformMatrix4fv(flatShaderPvm, 1, GL_FALSE, glm::value_ptr(pvm)));
			//CALL_GL(glDisable(GL_DEPTH_TEST));
			// draw the object
			CALL_GL(glBindVertexArray(picked));
			CALL_GL(glDrawElements(GL_TRIANGLES,selectedObject->numFaces*3,GL_UNSIGNED_INT,0));


			//Shorten the beam to match the object
			CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
			CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,5*sizeof(float),sizeof(rayLength),&rayLength));

			restoreRay = true;
		}
		if (picked == 0 && restoreRay == true) {
			CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,ray.vertexVboId));
			float d = -1000.0f;
			CALL_GL(glBufferSubData(GL_ARRAY_BUFFER,5*sizeof(float),sizeof(d),&d));
			restoreRay=false;
		}

	}
	CALL_GL(glEnable(GL_DEPTH_TEST));
	 

	if (appMode == rayCasting) {
		//draw plane for ray
		CALL_GL(glUseProgram(colorShader));
		/*pvm = projectionMatrix*viewMatrix*selectedObject->modelMatrix;
		CALL_GL(glUniformMatrix4fv(colorShaderPvm, 1, GL_FALSE, glm::value_ptr(pvm)));
		CALL_GL(glBindVertexArray(target.getVaoId()));
		CALL_GL(glDrawRangeElements(GL_TRIANGLES,0,42,42,GL_UNSIGNED_SHORT,NULL));*/


		//draw plane for ray then draw RAY
		pvm = projectionMatrix*viewMatrix*ray.modelMatrix;
		CALL_GL(glUniformMatrix4fv(colorShaderPvm, 1, GL_FALSE, glm::value_ptr(pvm)));
		CALL_GL(glLineWidth(3.5));
		CALL_GL(glBindVertexArray(plane.getVaoId()));
		CALL_GL(glDrawRangeElements(GL_LINES,0,12,8,GL_UNSIGNED_SHORT,NULL));

		CALL_GL(glBindVertexArray(ray.getVaoId()));
		CALL_GL(glDrawRangeElements(GL_LINES,0,6,2,GL_UNSIGNED_SHORT,NULL));
	}

	if (appMode == planeCasting) {
		CALL_GL(glUseProgram(colorShader));
		//draw plane
		pvm = projectionMatrix*viewMatrix*plane.modelMatrix*glm::scale(vec3(8,8,8));
		CALL_GL(glUniformMatrix4fv(colorShaderPvm, 1, GL_FALSE, glm::value_ptr(pvm)));
		CALL_GL(glBindVertexArray(plane.getVaoId()));
		CALL_GL(glDrawRangeElements(GL_LINES,0,12,8,GL_UNSIGNED_SHORT,NULL));
	}
	
	/*glUseProgram(textureShader);
	glUniformMatrix4fv(textureShaderPvm,1,GL_FALSE,glm::value_ptr(quad.modelMatrix));
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(textureShaderTexture,0);
	glBindVertexArray(quad.getVaoId());
	glDrawRangeElements(GL_TRIANGLES,0,24,24,GL_UNSIGNED_SHORT,NULL);
	*/
	CALL_GL(glUseProgram(dirLight));

	recursive_render(scene,scene->mRootNode);

	glfwSwapBuffers();
}

bool Engine::computeRotationMatrix() {

	vec3 H = glm::cross(ma,acc);

	float normH = (float) glm::length(H);

	if (normH<0.1f) return false;

	float invH = 1.0f / normH;

	H *= invH;

	float invA = 1.0f / (float) glm::length(acc);

	acc *= invA;

	vec3 M = glm::cross(acc,H);

	orientation[0][0] = H.x;   orientation[0][1] = H.y;   orientation[0][2] = H.z;
	orientation[1][0] = M.x;   orientation[1][1] = M.y;   orientation[1][2] = M.z;
	orientation[2][0] = acc.x; orientation[2][1] = acc.y; orientation[2][2] = acc.z;

	orientation = glm::rotate(orientation, glm::degrees((float)M_PI/2),vec3(1,0,0));
	orientation = glm::inverse(orientation);
	orientation = glm::rotate(orientation, glm::degrees((float)M_PI/2),vec3(1,0,0));

	if (calibrate) {
		calibration = glm::inverse(orientation);
		calibrate = !calibrate;
	}

	orientation = calibration*orientation;

	return true;
}

/*bool Engine::computeRotationMatrix() {
		//change = orientation;
		vec3 H = glm::cross(ma,acc);

		float normH = (float) H.length();

		if (normH<0.1f) return false;

		float invH = 1.0f / normH;

		H *= invH;

		float invA = 1.0f / (float) acc.length();

		acc *= invA;

		vec3 M = glm::cross(acc,H);

		orientation[0][0] = H.x;   orientation[0][1] = H.y;   orientation[0][2] = H.z;
		orientation[1][0] = M.x;   orientation[1][1] = M.y;   orientation[1][2] = M.z;
		orientation[2][0] = acc.x; orientation[2][1] = acc.y; orientation[2][2] = acc.z;

		glm::mat4 pitch = glm::mat4();
		pitch = glm::rotate(pitch,glm::degrees((float)M_PI_2),vec3(1,0,0));

		if(calibrate) { calibration = glm::inverse(orientation);
		calibrate = !calibrate; }

		orientation = mat4(axisChange)*calibration*orientation*pitch;
		return true;
	}*/

void Engine::mouseButtonCallback(int button, int state) {
	if ((button == 0) && (state == GLFW_PRESS)) {
		int xx,yy;
		glfwGetMousePos(&xx,&yy);
		arcBallPreviousPoint[0] = xx*1.0f;
		arcBallPreviousPoint[1] = yy*1.0f;
		tempOrigin = glm::vec3(selectedObject->modelMatrix[3][0],selectedObject->modelMatrix[3][1],selectedObject->modelMatrix[3][2]);
	}
}

void Engine::mouseMoveCallback(int xpos, int ypos) {
	//std::cout << "movse moved x:" << xpos << "\t y:" << ypos << std::endl;
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		selectedObject->modelMatrix = glm::translate(selectedObject->modelMatrix,-tempOrigin);
		glm::mat4 rot;
		rot = pho::util::getRotation(arcBallPreviousPoint[0],arcBallPreviousPoint[1],xpos*1.0f,ypos*1.0f, false);
		selectedObject->modelMatrix = rot*selectedObject->modelMatrix;
		selectedObject->modelMatrix = glm::translate(selectedObject->modelMatrix,tempOrigin);
		arcBallPreviousPoint[0] = xpos*1.0f;
		arcBallPreviousPoint[1] = ypos*1.0f;
	}
}

void Engine::go() {

	initResources();
	while(true) {
		checkEvents();
		render();
		if(glfwGetKey(GLFW_KEY_ESC)) {
			shutdown();
			break;
		}
	}
}

void Engine::shutdown() {
	netThread->interrupt();
#if defined(_DEBUG)
	_serialserver.shutDown();
#endif
	serialThread->interrupt();
	errorLog.close();
}

void Engine::addTuioObject(TuioObject *tobj) {
	if (verbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void Engine::updateTuioObject(TuioObject *tobj) {

	if (verbose)
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle()
		<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;

}

void Engine::removeTuioObject(TuioObject *tobj) {

	if (verbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << ")" << std::endl;
}

void Engine::addTuioCursor(TuioCursor *tcur) {

	//TUIO variables
	short numberOfCursors = tuioClient->getTuioCursors().size();
	std::list<TUIO::TuioCursor*> cursorList;
	cursorList = tuioClient->getTuioCursors();
	//std::cout << "Added cursor, Current NoOfCursors " << numberOfCursors << std::endl;

	switch (appInputState) {
	case idle:
		appInputState = translate;
		std::cout << "idle --> translate" << std::endl;
		break;
	case translate:
		/*if (numberOfCursors == 2) {
			appInputState = trackBall;

			trackedCursor = tcur;

			float x = tcur->getX();
			float y = tcur->getY();
			std::cout << "translate --> trackball" << std::endl;

			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			tempOrigin = glm::vec3(selectedObject->modelMatrix[3][0],selectedObject->modelMatrix[3][1],selectedObject->modelMatrix[3][2]);
		}*/
		break;
	case rotate:
		switch (rotTechnique) {
		case screenSpace:
			if (numberOfCursors == 1) {
				p1p.x = tcur->getX();
				p1p.y = tcur->getY();
				f1id = tcur->getCursorID();	
				
			}
			if (numberOfCursors == 2) {
				rotTechnique = pinch;
				std::cout << "pinch rotate" << '\n';
				p2p.x = tcur->getX();
				p2p.y = tcur->getY();
				f2id = tcur->getCursorID();
			}
			
			break;
		case trackBall:
			trackedCursorId = tcur->getCursorID();

			float x = tcur->getX();
			float y = tcur->getY();

			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			tempOrigin = glm::vec3(selectedObject->modelMatrix[3][0],selectedObject->modelMatrix[3][1],selectedObject->modelMatrix[3][2]);
		}
	}
	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void Engine::updateTuioCursor(TuioCursor *tcur) {
	glm::vec3 newLocationVector;
	float x,y;
	x = tcur->getX();
	y = tcur->getY();
	glm::mat3 tempMat;
	glm::mat4 newLocationMatrix;
	glm::mat4 rotation;
	tempOrigin = glm::vec3(selectedObject->modelMatrix[3][0],selectedObject->modelMatrix[3][1],selectedObject->modelMatrix[3][2]);
	glm::vec2 ftranslation;

	glm::vec2 f1translationVec,f2translationVec;
	glm::vec2 f1curr,f2curr;
	vec3 location;
	float newAngle;
	glm::vec2 ft;

	short numberOfCursors = tuioClient->getTuioCursors().size();
	std::list<TUIO::TuioCursor*> cursorList;
	cursorList = tuioClient->getTuioCursors();


	switch (appInputState) {
	case translate:
		//********************* TRANSLATE ****************************
		tempMat = glm::mat3(orientation);
#define TFACTOR 3
		x=(tcur->getXSpeed())/TFACTOR;
		y=(tcur->getYSpeed())/TFACTOR;
		newLocationVector = tempMat*glm::vec3(x,0,y);  //rotate the motion vector from TUIO in the direction of the plane
		//newLocationMatrix = glm::translate(selectedObject->modelMatrix,newLocationVector);  //Calculate new location by translating object by motion vector
		newLocationMatrix = glm::translate(glm::mat4(),newLocationVector);

		plane.modelMatrix = newLocationMatrix*plane.modelMatrix;
		selectedObject->modelMatrix = newLocationMatrix*selectedObject->modelMatrix;

		break;
		  
	
	case rotate:
		switch (rotTechnique) {
		case singleAxis:
			//todo: add code for aligned axis
			break;
		case screenSpace:
			//********************* screenSpace  *************************
			if (numberOfCursors == 1) {

				vec3 location;
				location.x = selectedObject->modelMatrix[3][0];
				location.y = selectedObject->modelMatrix[3][1];
				location.z = selectedObject->modelMatrix[3][2];  

				selectedObject->modelMatrix[3][0] = 0;
				selectedObject->modelMatrix[3][1] = 0;
				selectedObject->modelMatrix[3][2] = 0;

				selectedObject->modelMatrix = glm::rotate(tcur->getXSpeed()*3.0f,vec3(0,1,0))*selectedObject->modelMatrix;
				selectedObject->modelMatrix = glm::rotate(tcur->getYSpeed()*3.0f,vec3(1,0,0))*selectedObject->modelMatrix;

				selectedObject->modelMatrix[3][0] = location.x;
				selectedObject->modelMatrix[3][1] = location.y;
				selectedObject->modelMatrix[3][2] = location.z;
			}
			break;
		case pinch:
			//********************* PINCH  *************************


			if (tcur->getCursorID() == f1id) {
				p1c.x = tcur->getX();
				p1c.y = tcur->getY();
				p1t = p1c-p1p;
			}

			if (tcur->getCursorID() == f2id) {
				p2c.x = tcur->getX();
				p2c.y = tcur->getY();
				
				p2t = p2c-p2p;				
			}

			ft.x=std::max(0.0f,std::min(p1t.x,p2t.x)) + std::min(0.0f,std::max(p1t.x,p2t.x));
			ft.y=std::max(0.0f,std::min(p1t.y,p2t.y)) + std::min(0.0f,std::max(p1t.y,p1t.y));

			referenceAngle = atan2((p2p.y - p1p.y) ,(p2p.x - p1p.x)); 
			newAngle = atan2((p2c.y - p1c.y),(p2c.x - p1c.x));
			
			location.x = selectedObject->modelMatrix[3][0];
			location.y = selectedObject->modelMatrix[3][1];
			location.z = selectedObject->modelMatrix[3][2];

			selectedObject->modelMatrix[3][0] = 0;
			selectedObject->modelMatrix[3][1] = 0;
			selectedObject->modelMatrix[3][2] = 0;

			selectedObject->modelMatrix = glm::rotate((newAngle-referenceAngle)*(-10),vec3(0,0,1))*selectedObject->modelMatrix;
			
			selectedObject->modelMatrix = glm::rotate(ft.x*5,vec3(0,1,0))*selectedObject->modelMatrix;
			selectedObject->modelMatrix = glm::rotate(ft.y*5,vec3(1,0,0))*selectedObject->modelMatrix;
				
			selectedObject->modelMatrix[3][0] = location.x;
			selectedObject->modelMatrix[3][1] = location.y;
			selectedObject->modelMatrix[3][2] = location.z;

			
			//update to latest values
			//referenceAngle = newAngle; ???????????? doesn't seem to make a difference
			//if (tcur->getCursorID() == f1id) {	p1p = p1c; }
			//if (tcur->getCursorID() == f2id) {	p2p = p2c; }
			break;
		case trackBall:
			//********************* TRACKBALL  *************************
			x = tcur->getX();
			y = tcur->getY();

			selectedObject->modelMatrix = glm::translate(selectedObject->modelMatrix,-tempOrigin);

			rotation = pho::util::getRotation(arcBallPreviousPoint[0],arcBallPreviousPoint[1],x,y,true);
			selectedObject->modelMatrix = rotation*selectedObject->modelMatrix;
			selectedObject->modelMatrix = glm::translate(selectedObject->modelMatrix,tempOrigin);
			arcBallPreviousPoint[0] = x;
			arcBallPreviousPoint[1] = y;
			break;
		}
	}
	if (verbose)
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ") " << tcur->getX() << " " << tcur->getY()
		<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
	
}

void Engine::removeTuioCursor(TuioCursor *tcur) {

	short numberOfCursors = tuioClient->getTuioCursors().size()-1;

	//std::cout << "Removed cursor, Current NoOfCursors " << numberOfCursors << std::endl;

	switch (appInputState) {
	case translate:
		appInputState = idle;
		std::cout << "translate-->idle" << std::endl;
		break;
	case trackBall:
		/*if (numberOfCursors == 2) {
			trackedCursor = tuioClient->getTuioCursors().back();
			arcBallPreviousPoint[0] = tcur->getX();
			arcBallPreviousPoint[1] = tcur->getY();
			std::cout << "trackball --> trackball" << std::endl;
		}
		if (numberOfCursors == 1) {
			trackedCursor = tuioClient->getTuioCursors().back();
			arcBallPreviousPoint[0] = tcur->getX();
			arcBallPreviousPoint[1] = tcur->getY();
			std::cout << "trackball with 1 finger" << std::endl;
		}
		if (numberOfCursors == 0) {
			appInputState = idle;
			std::cout << "trackball --> idle " << std::endl;
		}*/

		break;
	case rotate:
		switch (rotTechnique) {
		case pinch:
			rotTechnique = screenSpace;
			std::cout << "pinch --> screenSpace" << '\n';
		}
	}

	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << ")" << std::endl;
}

void Engine::refresh(TuioTime frameTime) {
	//std::cout << "refresh " << frameTime.getTotalMilliseconds() << std::endl;
}

void Engine::generate_frame_buffer_texture() 
{  

	/* create a framebuffer object */ 
	CALL_GL(glGenFramebuffers(1, &fbo));     
	/* attach the texture and the render buffer to the frame buffer */ 
	CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo)); 

	/* generate a texture id */ 
	CALL_GL(glGenTextures(1, &tex)); 
	/* bind the texture */ 
	CALL_GL(glBindTexture(GL_TEXTURE_2D, tex)); 
	/* create the texture in the GPU */ 
	CALL_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_SIZE_X, WINDOW_SIZE_Y 
		, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr)); 

	/* set texture parameters */ 
	CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); 
	CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); 
	CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); 
	CALL_GL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)); 

	/* unbind the texture */ 
	CALL_GL(glBindTexture(GL_TEXTURE_2D, 0)); 

	/* create a renderbuffer object for the depth buffer */ 
	CALL_GL(glGenRenderbuffers(1, &rbo)); 
	/* bind the texture */ 
	CALL_GL(glBindRenderbuffer(GL_RENDERBUFFER, rbo)); 
	/* create the render buffer in the GPU */ 
	CALL_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT 
		, WINDOW_SIZE_X, WINDOW_SIZE_Y)); 

	/* unbind the render buffer */ 
	CALL_GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));


	/* attach the texture and the render buffer to the frame buffer */ 
	CALL_GL(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0)); 
	CALL_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT 
		, GL_RENDERBUFFER, rbo)); 

	// check the frame buffer 
	if (glCheckFramebufferStatus( 
		GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer status not complete" << '\n';
	}
	/* handle an error : frame buffer incomplete */ 
	/* return to the default frame buffer */ 
	CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 

	//CALL_GL(glEC("Texture Generation"));
}

GLuint Engine::picking(const struct aiScene *sc, const struct aiNode* nd) 
{ 
	GLubyte red, green, blue, alpha; 

	/* bind the frame buffer */ 
	CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo)); 

	/* clear the frame buffer */ 
	CALL_GL(glClearColor(0,0,0,0)); 
	CALL_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); 

	/* select the shader program */ 
	CALL_GL(glUseProgram(flatShader)); 
	GLuint tempVao = selectedObject->vaoId;

	alpha = heart.getVaoId() & 0xFF; 
	blue  = (tempVao >> 8) & 0xFF; 
	green = (tempVao >> 16) & 0xFF; 
	red   = (tempVao >> 24) & 0xFF; 
	CALL_GL(glUniform4f(flatShaderColor, red/255.0f, green/255.0f ,blue/255.0f, alpha/255.0f)); 	
	
	pvm = projectionMatrix*glm::inverse(ray.modelMatrix)*selectedObject->modelMatrix; //todo:this should be cycled through all objects!!!
	//pvm = projectionMatrix*viewMatrix*selectedObject->modelMatrix;
	CALL_GL(glUniformMatrix4fv(flatShaderPvm, 1, GL_FALSE, glm::value_ptr(pvm)));

	/* draw the object*/ 
	for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
		// bind VAO
		CALL_GL(glBindVertexArray(myMeshes[nd->mMeshes[n]].vao));
		// draw
		CALL_GL(glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0));
	}

	/*/ draw all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n){
		recursive_render(sc, nd->mChildren[n]);
	}*/

	//check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer Error" << '\n';
	}

	GLuint temp;

	temp = get_object_id();


	//glEC("off screen");

	/* return to the default frame buffer */
	CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 

	if (temp !=0) { 
		return temp; 
	}
	else return 0;
}

void Engine::generate_pixel_buffer_objects() 
{ 
	/* generate the pixel buffer object */ 
	CALL_GL(glGenBuffers(1,&pbo_a));     
	CALL_GL(glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_a)); 
	CALL_GL(glBufferData(GL_PIXEL_PACK_BUFFER, WINDOW_SIZE_X * WINDOW_SIZE_Y * 4, nullptr, GL_STREAM_READ)); 
	/* to avoid weird behaviour the first frame the data is loaded */ 
	CALL_GL(glReadPixels(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GL_BGRA, GL_UNSIGNED_BYTE, 0));     

	/* generate the first pixel buffer objects 
	glGenBuffers(1,&pbo_b);     
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_b); 
	glBufferData(GL_PIXEL_PACK_BUFFER, WINDOW_SIZE_X * WINDOW_SIZE_Y * 4, nullptr, GL_STREAM_READ); 
	// to avoid weird behaviour the first frame the data is loaded 
	glReadPixels(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GL_BGRA, GL_UNSIGNED_BYTE, 0);     
	// unbind 
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); */

}

GLuint Engine::get_object_id() 
{ 
	static int frame_event = 0; 
	GLuint object_id; 
	int x, y; 
	GLuint red, green, blue, alpha, pixel_index; 
	GLubyte* ptr; 

	/* switch between pixel buffer objects 
	if (frame_event == 0){ 
	frame_event = 1; 
	read_pbo = pbo_b; 
	map_pbo = pbo_a; 
	} 
	else { 
	frame_event = 0; 
	map_pbo = pbo_a; 
	read_pbo = pbo_b; 
	} */


	/* read one pixel buffer */ 
	CALL_GL(glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_a)); 
	/* map the other pixel buffer */  
	CALL_GL(glReadPixels(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GL_BGRA, GL_UNSIGNED_BYTE, 0));
	ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE); 
	/* get the mouse coordinates */ 
	/* OpenGL has the {0,0} at the down-left corner of the screen */ 
	x = WINDOW_SIZE_X/2;
	y = WINDOW_SIZE_Y/2;
	object_id = -1; 
	if (x >= 0 && x < WINDOW_SIZE_X && y >= 0 && y < WINDOW_SIZE_Y){ 
		//std::cout << "x - y " << '\t' << x << '\t' << y << '\n';
		pixel_index = (x + y * WINDOW_SIZE_X) * 4; 
		blue = ptr[pixel_index]; 
		green = ptr[pixel_index + 1]; 
		red = ptr[pixel_index + 2]; 
		alpha = ptr[pixel_index + 3]; 

		/*std::cout << "received : ";
		printf("0x%X\t", alpha);
		printf("0x%X\t", blue);
		printf("0x%X\t", green);
		printf("0x%X\n", red);*/

		object_id = alpha +(red << 24) + (green << 16) + (blue << 8);
	} 

	CALL_GL(glUnmapBuffer(GL_PIXEL_PACK_BUFFER)); 
	CALL_GL(glBindBuffer(GL_PIXEL_PACK_BUFFER, 0)); 
	return object_id; 
} 

/*GLuint Engine::get_object_id() 
{ 
static int frame_event = 0; 
GLuint object_id; 
int x, y; 
GLuint red, green, blue, alpha, pixel_index; 
GLubyte* ptr; 

/* switch between pixel buffer objects 
if (frame_event == 0){ 
frame_event = 1; 
read_pbo = pbo_b; 
map_pbo = pbo_a; 
} 
else { 
frame_event = 0; 
map_pbo = pbo_a; 
read_pbo = pbo_b; 
} 


// read one pixel buffer 
CALL_GL(glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_a)); 
// map the other pixel buffer 
CALL_GL(glReadPixels(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GL_BGRA, GL_UNSIGNED_BYTE, 0));
ptr = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE); 
// get the mouse coordinates 
// OpenGL has the {0,0} at the down-left corner of the screen 
glfwGetMousePos(&x, &y); 
//std::cout << x << " -" << y << '\n';
y = WINDOW_SIZE_Y - y; 
object_id = -1; 
if (x >= 0 && x < WINDOW_SIZE_X && y >= 0 && y < WINDOW_SIZE_Y){ 
//std::cout << "x - y " << '\t' << x << '\t' << y << '\n';
pixel_index = (x + y * WINDOW_SIZE_X) * 4; 
blue = ptr[pixel_index]; 
green = ptr[pixel_index + 1]; 
red = ptr[pixel_index + 2]; 
alpha = ptr[pixel_index + 3]; 

std::cout << "received : ";
printf("0x%X\t", alpha);
printf("0x%X\t", blue);
printf("0x%X\t", green);
printf("0x%X\n", red);

object_id = alpha +(red << 24) + (green << 16) + (blue << 8);
} 

CALL_GL(glUnmapBuffer(GL_PIXEL_PACK_BUFFER)); 
CALL_GL(glBindBuffer(GL_PIXEL_PACK_BUFFER, 0)); 
return object_id; 
} */


void Engine::glEC(const std::string place) 
{

	error = glGetError(); 
	if (error != 0) { 
		std::cout << "Error in " << place << " : ";
		switch(error) {
			//case GL_INVALID_ENUM​: std::cout << "Invalid Enum"; break; 
		case GL_INVALID_VALUE: std::cout << "Invalud Value"; break;
		case GL_INVALID_OPERATION: std::cout << "Invalid Operation"; break;
			//case GL_INVALID_FRAMEBUFFER_OPERATION​: std::cout << "Invalid Framebuffer Operation"; break;
		case GL_OUT_OF_MEMORY: std::cout << "Out of Memory"; break;
		default: std::cout << error; break;
		}
		std::cout << '\n';
	}

}

GLuint Engine::searchByName(const std::string name) {
	for (assetIterator = assets.begin(); assetIterator !=assets.end(); assetIterator++) {
		if(assetIterator->name==name) return assetIterator->vaoId;break;

	}
	return -1;
}

void Engine::initSimpleGeometry() {

	std::vector<GLushort> indices;
	vector<vec3> vertices;
	vector<vec3> colors;
	
	
	indices.push_back(0); indices.push_back(1); indices.push_back(3);
	indices.push_back(3); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(1); indices.push_back(8);

	indices.push_back(3); indices.push_back(2); indices.push_back(7);
	indices.push_back(7); indices.push_back(2); indices.push_back(6);
	indices.push_back(6); indices.push_back(2); indices.push_back(8);

	indices.push_back(7); indices.push_back(4); indices.push_back(6);
	indices.push_back(6); indices.push_back(4); indices.push_back(5);
	indices.push_back(5); indices.push_back(6); indices.push_back(8);

	indices.push_back(4); indices.push_back(5); indices.push_back(0);
	indices.push_back(0); indices.push_back(5); indices.push_back(1);
	indices.push_back(1); indices.push_back(5); indices.push_back(8);

	indices.push_back(0); indices.push_back(4); indices.push_back(3);
	indices.push_back(3); indices.push_back(4); indices.push_back(7);

	vertices.push_back(vec3(-0.5,-0.5,0.5)); //front bottom left
	colors.push_back(vec3(1.0,0.0,0.0));
	vertices.push_back(vec3(-0.5,0.5,0.5)); //front top left
	colors.push_back(vec3(0.0,1.0,0.0));
	vertices.push_back(vec3(0.5,0.5,0.5));  //front top right
	colors.push_back(vec3(0.0,0.0,1.0));
	vertices.push_back(vec3(0.5,-0.5,0.5)); //front bottom right
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(-0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(-0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0));
	vertices.push_back(vec3(0.5,0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(0.5,-0.5,-0.5));
	colors.push_back(vec3(1.0,1.0,1.0)); 
	vertices.push_back(vec3(0.0,1.0,0.5));	
	colors.push_back(vec3(1.0,1.0,1.0));

	target = pho::Asset(vertices,indices,colors,"target");
	assets.push_back(target);

	vertices.clear();
	
	vertices.push_back(vec3(-0.7,0,1));
	vertices.push_back(vec3(-0.7,0,-1));
	vertices.push_back(vec3(0.7,0,-1));
	vertices.push_back(vec3(0.7,0,1));

	colors.clear();
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));
	colors.push_back(vec3(0,0,1));

	indices.clear(); indices.push_back(0); indices.push_back(1);indices.push_back(1);indices.push_back(2);
	indices.push_back(2);indices.push_back(3);indices.push_back(3);indices.push_back(0);

	plane = pho::Asset(vertices,indices,colors,"plane");


	vertices.clear();
	colors.clear();
	indices.clear();

	vertices.push_back(vec3(0,0,0));
	colors.push_back(vec3(1,0,0));
	vertices.push_back(vec3(0,0,-1000));
	colors.push_back(vec3(1,1,1));

	indices.push_back(0);
	indices.push_back(1);

	ray = pho::Asset(vertices,indices,colors,"ray");


	indices.clear();
	vertices.clear();
	colors.clear();

	std::vector<glm::vec2> texcoords;

	vertices.push_back(vec3(-1,-1,0));
	texcoords.push_back(glm::vec2(0,0));

	vertices.push_back(vec3(-1, 1,0));
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1,-1,0));
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3( 1,-1,0));
	texcoords.push_back(glm::vec2(1,0));

	vertices.push_back(vec3(-1, 1,0));
	texcoords.push_back(glm::vec2(0,1));

	vertices.push_back(vec3( 1, 1,0));
	texcoords.push_back(glm::vec2(1,1));

	indices.push_back(0);indices.push_back(1);indices.push_back(2);
	indices.push_back(3);indices.push_back(4);indices.push_back(5);	

	quad = pho::Asset::Asset(vertices,indices,texcoords,"quad");

	quad.modelMatrix = glm::scale(glm::vec3(0.3,0.3,1));
	quad.modelMatrix = glm::translate(quad.modelMatrix,glm::vec3(-2.2,-2.2,0));
}

bool Engine::LoadGLTextures(const aiScene* scene)
{
	ILboolean success;

	/* initialization of DevIL */
	ilInit(); 

	/* scan scene's materials for textures */
	for (unsigned int m=0; m<scene->mNumMaterials; ++m)
	{
		int texIndex = 0;
		aiString path;	// filename

		aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		while (texFound == AI_SUCCESS) {
			//fill map with textures, OpenGL image ids set to 0
			textureIdMap[path.data] = 0; 
			// more textures?
			texIndex++;
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
		}
	}

	int numTextures = textureIdMap.size();

	/* create and fill array with DevIL texture ids */
	ILuint* imageIds = new ILuint[numTextures];
	ilGenImages(numTextures, imageIds); 

	/* create and fill array with GL texture ids */
	GLuint* textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

	/* get iterator */
	std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();
	int i=0;
	for (; itr != textureIdMap.end(); ++i, ++itr)
	{
		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second = textureIds[i];	  // save texture id for filename in map

		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT); 
		success = ilLoadImage((ILstring)filename.c_str());

		if (success) {
			/* Convert image to RGBA */
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 

			/* Create and load textures to OpenGL */
			glBindTexture(GL_TEXTURE_2D, textureIds[i]); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				ilGetData());
		}
		else 
			printf("Couldn't load Image: %s\n", filename.c_str());
	}
	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(numTextures, imageIds); 

	//Cleanup
	delete [] imageIds;
	delete [] textureIds;

	return true;
}