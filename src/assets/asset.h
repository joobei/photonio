#ifndef ASSET_H
#define ASSET_H

#include <OpenGL/gl3.h>
#include "glm/glm.hpp"
#include <vector>
#include "GLFW/glfw3.h"
#include "util.h"
#include "assimp/scene.h"
#include "boost/filesystem.hpp"
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include "boost/gil/gil_all.hpp"
#include "boost/gil/extension/io/png_io.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "shader.h"
#include <bullet/btBulletDynamicsCommon.h>

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
    GLuint shadowTexture;
    GLuint t1Location,t2Location,t3Location;
    //Physics
    btDynamicsWorld* dynamicsWorld =0;
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

inline GLuint nikoload(const std::string file_name)
{
    boost::gil::rgba8_image_t png;
    boost::gil::png_read_image(file_name,png);
    const auto view = boost::gil::const_view(png);
    const std::uint8_t* tex_ptr = boost::gil::interleaved_view_get_raw_data(view);
    ::GLuint tex_id=0;
    ::glGenTextures(1,&tex_id);
    ::glBindTexture(GL_TEXTURE_2D,tex_id);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    ::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    ::glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    ::glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                view.width(),
                view.height(),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                tex_ptr);

    return tex_id;
}
class Asset {

public:
    Asset();
    Asset(std::vector<glm::vec3> nvertices);
    Asset(const std::string &filename,pho::Shader* tehShader, sharedResources* shared, bool rigid);
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
    btRigidBody* rigidBody;
protected:
    void upload();
    glm::mat4* projectionMatrix;
    glm::mat4* biasMatrix;
    std::vector<MyMesh> meshes;
    const aiScene* scene;
    pho::Shader* shader;
    GLuint simpleVAO;
};
}

#endif
