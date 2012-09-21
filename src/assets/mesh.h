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
		Mesh(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec2> texcoords, std::string name);
		inline GLuint getVaoId();
		void render();
		glm::vec3 getPosition();
	private:
		glm::mat4 modelMatrix;
        GLuint vaoId,ibId,vertexVboId,texCoordVboId;
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

	
}

#endif 
