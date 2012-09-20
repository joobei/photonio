#ifndef ASSETS_H
#define ASSETS_H

#include "glm/glm.hpp"
#include <vector>
#include "GL/glew.h"
#include "GL/glfw.h"
#include "box.h"
#include "util.h"

namespace pho {


    class Mesh {
	public:
		Asset();
		Asset(std::string name);
		Asset(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors, std::string name);
		Asset(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec2> texcoords, std::string name);
		GLuint getVaoId();
		void render();
		glm::mat4 modelMatrix;
		GLuint* shader;
		GLuint vaoId,iboId,vertexVboId,colorVboId;
		std::vector<glm::vec3> vertices;
		std::vector<GLushort> indices;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec2> texcoords;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
		Box aabb;
		bool selected;
		std::string name;
		int numfaces;
		glm::vec3 getPosition();
	};

	struct MyMesh{
		GLuint vao;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
	};


	// Shader uniform block stuff *******************
	struct MyMaterial{

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float shininess;
		int texCount;
	};

	struct LightSource {
		glm::vec4 color;
		float attenuation;
		glm::vec3 direction;
		glm::vec3 position;
		float range;
		GLuint uniformBlockIndex;
	};

} }

#endif
