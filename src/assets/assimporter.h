#ifndef ASSIMPORTER_H
#define ASSIMPORTER_H

#include "model.h"
#include "assimp.hpp"  //assimp
#include "aiPostProcess.h"
#include "aiScene.h"
#include <fstream>
#include <iostream>
#include "assets.h"
#include "shader.h"
#include <map>
#include "IL/il.h" //devil for image loading

namespace Assimporter {
	
	static pho::Model Import(const char* filename);
	bool LoadGLTextures(const aiScene* scene);
	
	void set_float4(float f[4], float a, float b, float c, float d);

	void color4_to_float4(const struct aiColor4D *c, float f[4]);

}

#endif