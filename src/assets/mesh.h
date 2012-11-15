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

	enum PrimitiveType {
		Ray,
		Plane,
		Point
	};

	class Mesh {
	public:
		Mesh();
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors);
		Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz, pho::PrimitiveType type);
		GLuint getVaoId();
        void setShader(pho::Shader* tehShader);
		void draw();
		void drawPoint();
		void drawCircle();
		void draw(bool wireframe);
		void rotate(mat4 rotationMatrix);

		bool loadToGPU();
		glm::vec3 getPosition();
		void setPosition(const glm::vec3 &position);

		glm::vec3 farthestVertex;
		bool findIntersection(glm::mat4 rayMatrix, glm::vec3& foundPoint);
		bool findIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& foundPoint);
		bool rayTriangleIntersection(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::mat4 rayMatrix, float epsilon, glm::vec3 &intersection);
		bool rayTriangleIntersection(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 rayOrigin, glm::vec3 rayDirection, float epsilon, glm::vec3 &intersection);
		bool raySphereIntersection(const vec3& raydir, const vec3& rayorig,const vec3& pos,const float& rad, vec3& hitpoint,float& distance, vec3& normal);
		bool findSphereIntersection(glm::mat4 rayMatrix,glm::vec3& foundPoint,float& foundDistance,glm::vec3& foundNormal);
		bool findSphereIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& foundPoint,float& foundDistance,glm::vec3& foundNormal);
		float sum(const vec3& v);

		bool simple; //for rays, simple lines

		unsigned int numFaces, numVertices;
        GLuint vaoId;
		GLuint circlevaoId;
		GLuint wfvaoId;
		GLuint ibId;
		GLuint vertexVboId;
		GLuint colorVboId;
		GLuint normalVboId;
		GLuint wfibId; //indices for wireframe rendering
		GLuint texIndex;
		GLuint uniformBlockIndex;
		GLuint circleVboId;  //just one vertex to draw a point for the center of the circle

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
