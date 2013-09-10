#include "cursor.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

pho::Cursor::Cursor(sharedResources *sr)
{
    vec3 red = vec3(1,0,0);
    vec3 green = vec3(0,1,0);
    vec3 blue = vec3(0,0,1);
    vec3 yellow = vec3(1,1,0);

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


    btConvexHullShape* collisionShape = new btConvexHullShape();

    collisionShape->addPoint(btVector3(v0.x,v0.y,v0.z));
    collisionShape->addPoint(btVector3(v1.x,v1.y,v1.z));
    collisionShape->addPoint(btVector3(v2.x,v2.y,v2.z));
    collisionShape->addPoint(btVector3(v3.x,v3.y,v3.z));

    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(collisionShape);
    collisionObject->setUserPointer(this);

    vertices.push_back(v0);
    colors.push_back(red);
    colors.push_back(red);

    normals.push_back(v1-v0);
    normals.push_back(v1-v0);
    vertices.push_back(v1);

    vertices.push_back(v0);
    colors.push_back(green);
    colors.push_back(green);
    normals.push_back(v2-v0);
    normals.push_back(v2-v0);
    vertices.push_back(v2);

    vertices.push_back(v0);
    colors.push_back(blue);
    colors.push_back(blue);
    normals.push_back(v3-v0);
    normals.push_back(v3-v0);
    vertices.push_back(v3);

    vertices.push_back(v1);
    colors.push_back(yellow);
    colors.push_back(yellow);
    normals.push_back(v2-v1);
    normals.push_back(v2-v1);
    vertices.push_back(v2);

    vertices.push_back(v1);
    colors.push_back(red);
    colors.push_back(red);
    normals.push_back(v3-v1);
    normals.push_back(v3-v1);
    vertices.push_back(v3);

    vertices.push_back(v2);
    colors.push_back(blue);
    colors.push_back(blue);
    normals.push_back(v3-v2);
    normals.push_back(v3-v2);
    vertices.push_back(v3);

    vertices.push_back(v0); vertices.push_back(v2); vertices.push_back(v1);

    vec3 normal = glm::normalize(glm::cross((v1-v0),(v2-v0)));
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
    colors.push_back(green);
    colors.push_back(green);
    colors.push_back(green);

    vertices.push_back(v1); vertices.push_back(v2); vertices.push_back(v3);

    normal = glm::normalize(glm::cross((v3-v1),(v2-v1)));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        colors.push_back(blue);
        colors.push_back(blue);
        colors.push_back(blue);


    vertices.push_back(v3); vertices.push_back(v2); vertices.push_back(v0);

    normal = glm::normalize(glm::cross((v0-v3),(v2-v3)));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        colors.push_back(red);
        colors.push_back(red);
        colors.push_back(red);

    vertices.push_back(v3); vertices.push_back(v0); vertices.push_back(v1);

    normal = glm::normalize(glm::cross((v1-v3),(v0-v3)));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        colors.push_back(yellow);
        colors.push_back(yellow);
        colors.push_back(yellow);

    // generate Vertex Array for mesh
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint buffer;

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

    /*CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*normals.size(), normals.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(normalLoc));
    CALL_GL(glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0));*/

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*colors.size(), colors.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(colorLoc));
    CALL_GL(glVertexAttribPointer(colorLoc, 3, GL_FLOAT, 0, 0, 0));

}


void pho::Cursor::draw()
{
    //CALL_GL(glEnable(GL_CULL_FACE));
    //CALL_GL(glCullFace(GL_BACK));
    CALL_GL(glBindVertexArray(vao));
    res->colorShader.use();
    res->colorShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    res->colorShader["alpha"] = alpha;
    CALL_GL(glDrawArrays(GL_TRIANGLES,12,vertices.size()-12));
    //CALL_GL(glDisable(GL_CULL_FACE));

    //res->flatShader.use();
    //res->flatShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    //res->flatShader["color"] = vec4(1,1,1,1);
    CALL_GL(glLineWidth(5.0f));
    CALL_GL(glDrawArrays(GL_LINES,0,24));

}

void pho::Cursor::drawFromLight()
{
    res->flatLitShader.use();
    res->flatLitShader["mvp"] = res->projectionMatrix*res->light.viewMatrix*modelMatrix;
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glDrawArrays(GL_TRIANGLES,12,vertices.size()-12));
}

void pho::Cursor::drawLines(bool fromLight)
{
    CALL_GL(glBindVertexArray(vao));
    res->flatShader.use();
    if (fromLight)
    {
        res->flatShader["mvp"] = res->projectionMatrix*res->light.viewMatrix*modelMatrix;
        res->flatShader["color"] = vec4(0.5,0.5,0.5,1);
    }
    else
    {
        res->flatShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
        res->flatShader["color"] = vec4(0.5,0.5,0.5,1);
    }
    CALL_GL(glLineWidth(5.0f));
    CALL_GL(glDrawArrays(GL_LINES,0,24));
}

void pho::Cursor::setAlpha(float var)
{
    alpha = var;
}


pho::Ray::Ray(sharedResources *sr)
{
    vertices.push_back(glm::vec3(0,0,0));
    vertices.push_back(glm::vec3(0,0,-1000));
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    res = sr;
    CALL_GL(glGenBuffers(1, &vbo));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));
}

void pho::Ray::draw() {

    res->lineShader.use();
    res->lineShader["mvMat"] = res->viewMatrix*modelMatrix;
    res->lineShader["pMat"] = res->projectionMatrix;
    res->lineShader["radius"] = 0.02f;
    //res->lineShader["alpha"] = alpha;

    CALL_GL(glActiveTexture(GL_TEXTURE0));
    CALL_GL(glBindTexture(GL_TEXTURE_2D,texture));

    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glLineWidth(1));
    //glBlendFunc(GL_ONE, GL_ONE);  //try additive blending
    CALL_GL(glDrawArrays(GL_LINES,0,vertices.size()));
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void pho::Ray::setAlpha(float var)
{
    alpha = var;
}
