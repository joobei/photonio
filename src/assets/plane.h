#ifndef PLANE_H
#define PLANE_H

#include "asset.h"
#include "shader.h"

namespace pho {

class Plane : public Asset {
public:
    Plane();
    void extendToObject(pho::Asset &toMatch);
    void draw();
private:
    void upload();
    std::vector<glm::vec3> vertices;
    GLuint vao;
};

}
#endif
