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
    GLuint gradientTexture;
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    GLuint vao;
};

}
#endif
