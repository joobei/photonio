#ifndef ASSIMPORTER_H
#define ASSIMPORTER_H

#include "model.h"
#include "assimp.hpp"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <fstream>
#include <iostream>


namespace pho {
	class Assimporter {
	public:
		static pho::Model Import(const char* filename);
	private:
		aiScene* scene;
	}
}

#endif