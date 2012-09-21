#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>
#include <mesh.h>

namespace pho {

	class Model {
	public:
		Model(std::vector<pho::Mesh>);
		void draw();
	private:
		std::vector<pho::Mesh> meshes; 
	};
}

#endif