#ifndef ASSET_H
#define ASSET_H

#include "glm/glm.hpp"
#include <vector>
#include "GL/glew.h"
#include "GL/glfw.h"
#include "util.h"
#include "assimp/scene.h"
#include "boost/filesystem.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "shader.h"
#include "gli/gli.hpp"


namespace pho {

enum DrawingMode {
    normal,
    wireframe,
    transparent
};

class  LightSource {
public:
    glm::vec4 color;
    float attenuation;
    glm::vec3 direction;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    float range;
    GLuint uniformBlockIndex;
};

struct myMesh {
    GLuint vao;
    int numFaces;
    uint materialIndex;
};

class Asset {

public:
    Asset();
    Asset(const std::string &filename);
    void draw();
    glm::mat4 modelMatrix;
    void rotate(glm::mat4 rotationMatrix);
private:
    void upload();
    std::vector<myMesh> mMeshes;
    const aiScene* scene;
};
}

#endif
