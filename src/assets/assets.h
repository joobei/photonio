#ifndef ASSETS_H
#define ASSETS_H

#include "glm/glm.hpp"
#include <vector>
#include "GL/glew.h"
#include "GL/glfw.h"
#include "box.h"
#include "util.h"

namespace pho {

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
