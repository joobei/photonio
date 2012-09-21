#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include <vector>
#include "GL/glew.h"
#include "shader.h"

namespace pho {

	class Mesh {
	public:
		Mesh();
		Mesh(std::string name);
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors, std::string name);
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec2> texcoords, std::string name);
		inline GLuint getVaoId();
		void render();
		glm::vec3 getPosition();
	private:
		glm::mat4 modelMatrix;
		GLuint vaoId,iboId,vertexVboId,colorVboId;
		std::vector<glm::vec3> vertices;
		std::vector<GLushort> indices;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec2> texcoords;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
		bool selected;
		std::string name;
		int numfaces;
		Shader *shader;
	}; 

	// Vertex Attribute Locations
	const GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2, colorLoc=3;

	// Uniform Bindings Points
	const GLuint matricesUniLoc = 1, materialUniLoc = 2, lightUniLoc = 3;
}

#endif 