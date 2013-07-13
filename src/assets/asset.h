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
#include "gli/gtx/gl_texture2d.hpp"


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

struct myMaterial {
    GLuint diffuseTexture = 0;
    GLuint normalTexture = 0;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    float shininess;
    bool hasBumpMap = false;
};

class MyMesh {
public:
    MyMesh();
    GLuint vao;
    std::vector<int> indices;
    int numFaces;
    pho::myMaterial material;
    pho::Shader* shader;
};

class Asset {

public:
    Asset();
    Asset(const std::string &filename,pho::Shader* tehShader);
    void draw();
    glm::mat4 modelMatrix;
    glm::mat4 scaleMatrix;
    void rotate(glm::mat4 rotationMatrix);
    void setShader(pho::Shader* tehShader);
    void setPosition(glm::vec3 position);
    void setScale(float scaleFactor);
private:

    void upload(pho::Shader* tehShader);
    std::vector<MyMesh> mMeshes;
    const aiScene* scene;

};
}

#endif
