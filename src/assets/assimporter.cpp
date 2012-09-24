#pragma warning(disable: 4819)

#include "assimporter.h"

pho::Model Assimporter::Import(const char* filename) {

	pho::Model importedModel;

	const aiScene* _theScene = NULL;
	std::map<std::string, GLuint> textureIdMap;
	std::vector<pho::Mesh> meshes;
	//*********** ASSIMP LOADING CODE****************

	/*glGenBuffers(1,&matricesUniBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
	glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc, matricesUniBuffer, 0, MatricesUniBufferSize);*/

	// Create an instance of the Importer class
	Assimp::Importer importer;

	std::string pFile;
	pFile = filename;

	//check if file exists
	std::ifstream fin(pFile.c_str());
	if(!fin.fail()) {
		fin.close();
	}  
	else{
		std::cout << "Couldn't open file:"  << pFile << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
	}

	_theScene = &aiScene();

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	_theScene = importer.ReadFile( pFile,
		aiProcess_CalcTangentSpace       |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType );

	_theScene = importer.GetOrphanedScene();

	LoadGLTextures(_theScene);

	// If the import failed, report it
	if( !_theScene) {
		std::cout <<  importer.GetErrorString() << std::endl;
	}
	else { 
		std::cout << "Import of geometry succeeded." << std::endl; 
	}



	std::cout << "Number of meshes : " << _theScene->mNumMeshes << '\n';

	// For each mesh
	for (unsigned int n = 0; n < _theScene->mNumMeshes; ++n)
	{
		//loop through meshes
		pho::Mesh aMesh;
		pho::Material aMat;
		GLuint buffer;
		const struct aiMesh* mesh = _theScene->mMeshes[n];

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
		aMesh.numFaces = _theScene->mMeshes[n]->mNumFaces;

		// generate Vertex Array for mesh
		glGenVertexArrays(1,&(aMesh.vaoId));
		glBindVertexArray(aMesh.vaoId);

		// buffer for faces
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);


		// buffer for vertex positions
		if (mesh->HasPositions()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(pho::vertexLoc);
			glVertexAttribPointer(pho::vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// buffer for vertex normals
		if (mesh->HasNormals()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(pho::normalLoc);
			glVertexAttribPointer(pho::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
			glEnableVertexAttribArray(pho::texCoordLoc);
			glVertexAttribPointer(pho::texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		// create material uniform buffer
		struct aiMaterial *mtl = _theScene->mMaterials[mesh->mMaterialIndex];

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

		meshes.push_back(aMesh);
		std::cout << "Added mesh :" << n << std::endl;
		std::cout << "Number of faces :" << aMesh.numFaces << std::endl;

		}
		//LOAD TEXTURES ***********************

		ILboolean success;

		/* initialization of DevIL */
		ilInit(); 

		/* scan scene's materials for textures */
		for (unsigned int m=0; m<_theScene->mNumMaterials; ++m)
		{
			int texIndex = 0;
			aiString path;	// filename

			aiReturn texFound = _theScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			while (texFound == AI_SUCCESS) {
				//fill map with textures, OpenGL image ids set to 0
				textureIdMap[path.data] = 0; 
				// more textures?
				texIndex++;
				texFound = _theScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
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

		

	
}

//// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.

void Assimporter::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void Assimporter::color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}