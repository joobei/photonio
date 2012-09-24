#pragma warning(disable: 4819)

#include "model.h"

pho::Model::Model(std::vector<pho::Mesh> meshez) {

}

pho::Model::Model() {
}

void pho::Model::addMesh(pho::Mesh newMesh) {
	meshes.push_back(newMesh);
}