#include "plane.h"

pho::Plane::Plane()
{
    vertices.push_back(glm::vec3(0.3,0,-0.5));
    vertices.push_back(glm::vec3(0.3,0,0.5));
    vertices.push_back(glm::vec3(-0.3,0,0.5));
    vertices.push_back(glm::vec3(-0.3,0,-0.5));
    upload();
}


void pho::Plane::upload()
{
    // generate Vertex Array for mesh
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint buffer;

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

}


void pho::Plane::draw()
{
    CALL_GL(glLineWidth(5));
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glDrawArrays(GL_LINE_LOOP,0,vertices.size()));
    CALL_GL(glLineWidth(1));
}


