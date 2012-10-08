#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <vector>
#include "GL/glew.h"
#include "shader.h"
#include "assets.h"
#include "util.h"

namespace pho {

	class Mesh {
	public:
		Mesh();
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors);
		inline GLuint getVaoId();
        void setShader(pho::Shader* tehShader);
		void draw();
		bool loadToGPU();
		glm::vec3 getPosition();
		unsigned int numFaces, numVertices;
        GLuint vaoId,ibId,vertexVboId,colorVboId;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		unsigned int* faces;
		glm::mat4 modelMatrix;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLushort> indices;	 
		std::vector<glm::vec3> colors;
		bool selected;
		std::string name;
        pho::Shader* shader;
		bool uploaded;
		float* texCoords;
		pho::Material material;
	}; 

	
}

#endif 
