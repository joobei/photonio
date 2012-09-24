#ifndef ASSIMPORTER_H
#define ASSIMPORTER_H

#include "model.h"
#include "assimp.hpp"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <fstream>
#include <iostream>
#include "assets.h"

namespace Assimporter {
	
	static pho::Model Import(const char* filename);
	void LoadGLTextures(const aiScene* scene);
	
	void Assimporter::set_float4(float f[4], float a, float b, float c, float d);

	void Assimporter::color4_to_float4(const struct aiColor4D *c, float f[4]);

}

#endif