#include "cursor.h"

pho::Cursor::Cursor(sharedResources *sr)
{
    res = sr;

    v0 = glm::vec3(0,-0.5,0.58);
    v1 = glm::vec3(0.5,-0.5,-0.29);
    v2 = glm::vec3(-0.5,-0.5,-0.29);
    v3 = glm::vec3(0,0.5,0);

    glm::vec3 c0 = glm::vec3(1,0,0);
    glm::vec3 c1 = glm::vec3(0,1,0);
    glm::vec3 c2 = glm::vec3(0,0,1);
    glm::vec3 c3 = glm::vec3(1,1,0);

    vertices.push_back(v0); colors.push_back(c0);
    vertices.push_back(v1); colors.push_back(c1);
    vertices.push_back(v2); colors.push_back(c2);
    vertices.push_back(v3); colors.push_back(c3);

    btConvexHullShape* collisionShape = new btConvexHullShape();

    collisionShape->addPoint(btVector3(v0.x,v0.y,v0.z));
    collisionShape->addPoint(btVector3(v1.x,v1.y,v1.z));
    collisionShape->addPoint(btVector3(v2.x,v2.y,v2.z));
    collisionShape->addPoint(btVector3(v3.x,v3.y,v3.z));

    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(collisionShape);
    collisionObject->setUserPointer(this);

    //for the LINES
    //vertices.push_back(v0); colors.push_back(c0);
    //vertices.push_back(v1); colors.push_back(c1);

    vertices.push_back(v0); colors.push_back(c0);
    vertices.push_back(v2); colors.push_back(c2);

    vertices.push_back(v1); colors.push_back(c1);
    vertices.push_back(v2); colors.push_back(c2);

    //vertices.push_back(v2); colors.push_back(c2);
    //vertices.push_back(v3); colors.push_back(c3);

    vertices.push_back(v3); colors.push_back(c3);
    vertices.push_back(v0); colors.push_back(c0);

    vertices.push_back(v1); colors.push_back(c1);
    vertices.push_back(v3); colors.push_back(c3);


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
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*colors.size(), colors.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(colorLoc));
    CALL_GL(glVertexAttribPointer(colorLoc, 3, GL_FLOAT, 0, 0, 0));
}


void pho::Cursor::draw()
{
    /*res->colorShader.use();
    res->colorShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    res->colorShader["alpha"] = 1.0f;
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glLineWidth(5));
    CALL_GL(glDrawArrays(GL_LINES,0,vertices.size()));
    CALL_GL(glLineWidth(1));
    CALL_GL(glPointSize(20));
    CALL_GL(glDrawArrays(GL_POINTS,0,4));*/

    res->lineShader.use();
    res->lineShader["mvMat"] = res->viewMatrix*modelMatrix*glm::mat4(2);
    res->lineShader["pMat"] = res->projectionMatrix;
    res->lineShader["radius"] = 0.1f;

    CALL_GL(glActiveTexture(GL_TEXTURE0));
    CALL_GL(glBindTexture(GL_TEXTURE_2D,gradientTexture));

    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glLineWidth(1));
    CALL_GL(glDrawArrays(GL_LINES,0,vertices.size()));

    res->colorShader.use();
    res->colorShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    res->colorShader["alpha"] = 1.0f;
    CALL_GL(glDrawArrays(GL_POINTS,0,4));

}

void pho::Cursor::drawFromLight()
{
    res->flatShader.use();
    res->flatShader["mvp"] = res->projectionMatrix*res->light.viewMatrix*modelMatrix;
    res->flatShader["color"] = glm::vec4(1,1,1,1);
    CALL_GL(glBindVertexArray(vao));
    CALL_GL(glLineWidth(5));
    CALL_GL(glDrawArrays(GL_LINE_LOOP,0,vertices.size()));
    CALL_GL(glLineWidth(1));
    CALL_GL(glPointSize(20));
    CALL_GL(glDrawArrays(GL_POINTS,0,4));

}
