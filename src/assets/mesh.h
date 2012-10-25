#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
#include <vector>
#include "GL/glew.h"
#include "shader.h"
#include "assets.h"
#include "util.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

namespace pho {

	struct Face {
		GLshort a,b,c;
		glm::vec3 normal;
	};

	class Mesh {
	public:
		Mesh();
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors);
		Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz, bool simple);
		GLuint getVaoId();
        void setShader(pho::Shader* tehShader);
		void draw();
		void draw(bool wireframe);

		bool loadToGPU();
		glm::vec3 getPosition();
		
		glm::vec3 farthestVertex;
		bool findIntersection(glm::mat4 rayMatrix, glm::vec3& foundPoint);
		bool rayTriangleIntersection(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::mat4 rayMatrix, float epsilon, glm::vec3 &intersection);
		bool findSphereIntersection(glm::mat4 rayMatrix,glm::vec3& foundPoint,glm::vec3& foundNormal);


		bool simple; //for rays, simple lines

		unsigned int numFaces, numVertices;
        GLuint vaoId;
		GLuint wfvaoId;
		GLuint ibId;
		GLuint vertexVboId;
		GLuint colorVboId;
		GLuint wfibId; //indices for wireframe rendering
		GLuint texIndex;
		GLuint uniformBlockIndex;

		std::vector<pho::Face> faces;
		glm::mat4 modelMatrix;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLushort> indices;	 
		std::vector<GLushort> wfindices;	 
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
