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

    btCollisionObject* collisionObject;
    void drawFromLight();
    void drawLines(bool fromLight);
    GLuint gradientTexture;
    void setAlpha(float var);
    glm::vec3 color;
protected:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;
    float alpha;
    GLuint vao;
};

class Ray : public Asset {
public:
    Ray(sharedResources *sr);
    void draw();
    GLuint texture;
    void setAlpha(float var);
    GLuint vbo;
protected:
    float alpha;
    GLuint vao;


};

}
#endif
