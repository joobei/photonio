#ifndef CURSOR_H
#define CURSOR_H

#include "asset.h"
#include "shader.h"

namespace pho {

class Cursor : public Asset {
public:
    Cursor(sharedResources *sr);
    void draw();
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;

    glm::vec3 m0;
    glm::vec3 m1;
    glm::vec3 m2;
    glm::vec3 m3;
    glm::vec3 m4;
    glm::vec3 m5;

    glm::vec3 d0;
    glm::vec3 d1;
    glm::vec3 d2;
    glm::vec3 d3;
    glm::vec3 d4;
    glm::vec3 d5;

    btCollisionObject* collisionObject;
    void drawFromLight();
    GLuint gradientTexture;
    void setAlpha(float var);
protected:
    int vertexCount = 122;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    float alpha;
    glm::vec3 color;
    GLuint vao;
    GLuint spherevao;
};

class Ray : public Asset {
public:
    Ray(sharedResources *sr);
    void draw();
    GLuint texture;
    void setAlpha(float var);
protected:
    float alpha;
    GLuint vao;

};

}
#endif
