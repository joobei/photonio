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
#include <btBulletCollisionCommon.h>

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

struct sharedResources {
    LightSource light;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 biasMatrix;
    glm::mat4 shadowMatrix;
    pho::Shader flatShader;
    pho::Shader colorShader;
    pho::Shader lineShader;
    pho::Shader tubeShader;
    pho::Shader cylinderShader;
    btCollisionWorld*  collisionWorld;
    GLuint shadowTexture;
    GLuint t1Location,t2Location,t3Location;
};

struct myMaterial {
    GLuint diffuseTexture = 0;
    GLuint normalTexture = 0;
    glm::vec4 diffuseColor;
    glm::vec3 specularColor;
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
    Asset(std::vector<glm::vec3> nvertices);
    Asset(const std::string &filename,pho::Shader* tehShader, sharedResources* shared);
    void draw();
    void drawFlat();
    void drawFromLight();
    void scale();
    glm::mat4 modelMatrix;
    glm::mat4 scaleMatrix;
    void rotate(glm::mat4 rotationMatrix);
    void setFlatShader(pho::Shader* tehShader);
    void setPosition(glm::vec3 position);
    void setScale(float scaleFactor);
    std::vector<glm::vec3> vertices;
    bool beingIntersected;
    bool receiveShadow;
    glm::mat4* viewMatrix; //public because shadow map render function updates it to the pointlight matrix temporarily
    sharedResources* res;
    btCollisionObject collisionObject;
protected:
    void upload();
    glm::mat4* projectionMatrix;
    glm::mat4* biasMatrix;
    std::vector<MyMesh> mMeshes;
    const aiScene* scene;
    pho::Shader* shader;
    GLuint simpleVAO;
};
}

#endif
