#include "cursor.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

pho::Cursor::Cursor(sharedResources *sr)
{
    res = sr;

    /*v0 = vec3(0,-0.5,0.58);
    v1 = vec3(0.5,-0.5,-0.29);
    v2 = vec3(-0.5,-0.5,-0.29);
    v3 = vec3(0,0.5,0);*/

    v0 = vec3(1,0,-1/sqrt(2));
    v1 = vec3(-1,0,-1/sqrt(2));
    v2 = vec3(0,1,1/sqrt(2));
    v3 = vec3(0,-1,1/sqrt(2));

    v0 = mat3(glm::rotate(mat4(), (glm::mediump_float)-35, vec3(1,0,0)))*v0;
    v1 = mat3(glm::rotate(mat4(), (glm::mediump_float)-35, vec3(1,0,0)))*v1;
    v2 = mat3(glm::rotate(mat4(), (glm::mediump_float)-35, vec3(1,0,0)))*v2;
    v3 = mat3(glm::rotate(mat4(), (glm::mediump_float)-35, vec3(1,0,0)))*v3;


    m0 = vec3((v1.x+v0.x)/2,(v1.y+v0.y)/2,(v1.z+v0.z)/2);
    m1 = vec3((v2.x+v3.x)/2,(v2.y+v3.y)/2,(v2.z+v3.z)/2);
    m2 = vec3((v2.x+v0.x)/2,(v2.y+v0.y)/2,(v2.z+v0.z)/2);
    m3 = vec3((v1.x+v2.x)/2,(v1.y+v2.y)/2,(v1.z+v2.z)/2);
    m4 = vec3((v3.x+v0.x)/2,(v3.y+v0.y)/2,(v3.z+v0.z)/2);
    m5 = vec3((v1.x+v3.x)/2,(v1.y+v3.y)/2,(v1.z+v3.z)/2);

    d0 = vec3(v1-v0);
    d1 = vec3(v3-v2);
    d2 = vec3(v0-v2);
    d3 = vec3(v2-v1);
    d4 = vec3(v3-v0);
    d5 = vec3(v3-v1);

    vec3 red = vec3(1,0,0);
    vec3 green = vec3(0,1,0);
    vec3 blue = vec3(0,0,1);
    vec3 yellow = vec3(1,1,0);

    btConvexHullShape* collisionShape = new btConvexHullShape();

    collisionShape->addPoint(btVector3(v0.x,v0.y,v0.z));
    collisionShape->addPoint(btVector3(v1.x,v1.y,v1.z));
    collisionShape->addPoint(btVector3(v2.x,v2.y,v2.z));
    collisionShape->addPoint(btVector3(v3.x,v3.y,v3.z));

    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(collisionShape);
    collisionObject->setUserPointer(this);

    float radius = 0.05;

    vec3 toMultiply = vec3(1,3,-9);
    toMultiply = glm::normalize(toMultiply);

    //d0 represents vector between two vertices
    //multiply that with any other vector to get vertical with cross product

    vec3 rotater = glm::cross(d0,toMultiply);
    rotater = glm::normalize(rotater);

    int divisions = 60;
    //rotate vertical vector around d0 to get points on the base of the cylinder
    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v0+radius*rotater);
        vertexCount++;
        normals.push_back(rotater);
        vertices.push_back(v0+d0+radius*rotater);
        vertexCount++;
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d0))*rotater;

    }


    rotater = glm::cross(d1,toMultiply);
    rotater = glm::normalize(rotater);

    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v2+radius*rotater);
        normals.push_back(rotater);
        vertices.push_back(v2+d1+radius*rotater);
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d1))*rotater;
    }

    rotater = glm::cross(d2,toMultiply);
    rotater = glm::normalize(rotater);

    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v2+radius*rotater);
        normals.push_back(rotater);
        vertices.push_back(v2+d2+radius*rotater);
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d2))*rotater;
    }

    rotater = glm::cross(d3,toMultiply);
    rotater = glm::normalize(rotater);

    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v1+radius*rotater);
        normals.push_back(rotater);
        vertices.push_back(v1+d3+radius*rotater);
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d3))*rotater;
    }

    rotater = glm::cross(d4,toMultiply);
    rotater = glm::normalize(rotater);

    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v0+radius*rotater);
        normals.push_back(rotater);
        vertices.push_back(v0+d4+radius*rotater);
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d4))*rotater;
    }

    rotater = glm::cross(d5,toMultiply);
    rotater = glm::normalize(rotater);

    for (int i = 0;i < divisions+1;++i) {
        vertices.push_back(v1+radius*rotater);
        normals.push_back(rotater);
        vertices.push_back(v1+d5+radius*rotater);
        normals.push_back(rotater);
        rotater = glm::mat3(glm::rotate(glm::mat4(),(float)360/divisions,d5))*rotater;
    }

    // generate Vertex Array for mesh
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint buffer;

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*normals.size(), normals.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(normalLoc));
    CALL_GL(glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0));

}


void pho::Cursor::draw()
{
    res->flatLitShader.use();
    res->flatLitShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    res->flatLitShader["modelview"] = res->viewMatrix*modelMatrix;
    res->flatLitShader["material_diffuse"] = vec4(0.6,0.3,0.8,1);
    res->flatLitShader["material_specular"] = vec4(1,1,1,1);
    res->flatLitShader["material_shininess"] = 200.0f;
    res->flatLitShader["light_position"] = vec4(res->light.position,1.f);
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,0,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*2,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*3,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*4,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*5,vertexCount));
}

void pho::Cursor::drawFromLight()
{
    res->flatShader.use();
    res->flatShader["mvp"] = res->projectionMatrix*res->light.viewMatrix*modelMatrix;
    res->flatShader["color"] = vec4(1,1,1,1);
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,0,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*2,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*3,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*4,vertexCount));
    CALL_GL(glDrawArrays(GL_TRIANGLE_STRIP,vertexCount*5,vertexCount));

}
