#include "cursor.h"

pho::Cursor::Cursor(sharedResources *sr)
{
    res = sr;

    /*v0 = glm::vec3(0,-0.5,0.58);
    v1 = glm::vec3(0.5,-0.5,-0.29);
    v2 = glm::vec3(-0.5,-0.5,-0.29);
    v3 = glm::vec3(0,0.5,0);*/

    v0 = glm::vec3(1,0,-1/sqrt(2));
    v1 = glm::vec3(-1,0,-1/sqrt(2));
    v2 = glm::vec3(0,1,1/sqrt(2));
    v3 = glm::vec3(0,-1,1/sqrt(2));

    v0 = glm::mat3(glm::rotate(glm::mat4(), (glm::mediump_float)-35, glm::vec3(1,0,0)))*v0;
    v1 = glm::mat3(glm::rotate(glm::mat4(), (glm::mediump_float)-35, glm::vec3(1,0,0)))*v1;
    v2 = glm::mat3(glm::rotate(glm::mat4(), (glm::mediump_float)-35, glm::vec3(1,0,0)))*v2;
    v3 = glm::mat3(glm::rotate(glm::mat4(), (glm::mediump_float)-35, glm::vec3(1,0,0)))*v3;


    m0 = glm::vec3((v1.x+v0.x)/2,(v1.y+v0.y)/2,(v1.z+v0.z)/2);
    m1 = glm::vec3((v2.x+v3.x)/2,(v2.y+v3.y)/2,(v2.z+v3.z)/2);
    m2 = glm::vec3((v2.x+v0.x)/2,(v2.y+v0.y)/2,(v2.z+v0.z)/2);
    m3 = glm::vec3((v1.x+v2.x)/2,(v1.y+v2.y)/2,(v1.z+v2.z)/2);
    m4 = glm::vec3((v3.x+v0.x)/2,(v3.y+v0.y)/2,(v3.z+v0.z)/2);
    m5 = glm::vec3((v1.x+v3.x)/2,(v1.y+v3.y)/2,(v1.z+v3.z)/2);

    d0 = glm::vec3(v1-v0);
    d1 = glm::vec3(v3-v2);
    d2 = glm::vec3(v0-v2);
    d3 = glm::vec3(v2-v1);
    d4 = glm::vec3(v3-v0);
    d5 = glm::vec3(v3-v1);

    glm::vec3 c0 = glm::vec3(1,0,0);
    glm::vec3 c1 = glm::vec3(0,1,0);
    glm::vec3 c2 = glm::vec3(0,0,1);
    glm::vec3 c3 = glm::vec3(1,1,0);



    btConvexHullShape* collisionShape = new btConvexHullShape();

    collisionShape->addPoint(btVector3(v0.x,v0.y,v0.z));
    collisionShape->addPoint(btVector3(v1.x,v1.y,v1.z));
    collisionShape->addPoint(btVector3(v2.x,v2.y,v2.z));
    collisionShape->addPoint(btVector3(v3.x,v3.y,v3.z));

    collisionObject = new btCollisionObject();
    collisionObject->setCollisionShape(collisionShape);
    collisionObject->setUserPointer(this);

    //midpoints
    vertices.push_back(m0);
    vertices.push_back(m1);
    vertices.push_back(m2);
    vertices.push_back(m3);
    vertices.push_back(m4);
    vertices.push_back(m5);


    /*for(float i = 0; i < 6.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi
    {
        vertices.push_back(glm::vec3(radius*cos(i),radius*sin(i),0.5));
        rayVerticesCount++;
        vertices.push_back(glm::vec3(radius*cos(i),radius*sin(i),-0.5));
        rayVerticesCount++;
    }*/


    // generate Vertex Array for mesh
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint buffer;

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));

    vertices.clear();

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    glGenVertexArrays(1,&spherevao);
    glBindVertexArray(spherevao);

    CALL_GL(glGenBuffers(1, &buffer));
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    CALL_GL(glEnableVertexAttribArray(vertexLoc));
    CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));
}


void pho::Cursor::draw()
{
    CALL_GL(glBindVertexArray(vao));
    res->cylinderShader.use();
    res->cylinderShader["CylinderExt"] = 1.f;
    res->cylinderShader["CylinderRadius"] = .1f ;
    res->cylinderShader["MVMatrix"] = res->viewMatrix*modelMatrix;
    res->cylinderShader["PMatrix"] = res->projectionMatrix;
    res->cylinderShader["NormalMatrix"] = glm::mat3(res->viewMatrix*modelMatrix);
    res->cylinderShader["lightPos"] = glm::vec4(0,5,5,1);
    res->cylinderShader["EyePoint"] = res->viewMatrix[3];
    res->cylinderShader["color"] = glm::vec4(1.f,0.f,0.f,0.5f);

    res->cylinderShader["CylinderDirection"] = d0;
    CALL_GL(glDrawArrays(GL_POINTS,0,1));

    res->cylinderShader["CylinderDirection"] = d1;
    CALL_GL(glDrawArrays(GL_POINTS,1,1));

    res->cylinderShader["CylinderDirection"] = d2;
    CALL_GL(glDrawArrays(GL_POINTS,2,1));

    res->cylinderShader["CylinderDirection"] = d3;
    CALL_GL(glDrawArrays(GL_POINTS,3,1));

    res->cylinderShader["CylinderDirection"] = d4;
    CALL_GL(glDrawArrays(GL_POINTS,4,1));

    res->cylinderShader["CylinderDirection"] = d5;
    CALL_GL(glDrawArrays(GL_POINTS,5,1));


    CALL_GL(glBindVertexArray(spherevao));
    res->flatShader.use();
    res->flatShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    float alpha = 1.0f;
    res->flatShader["color"] = glm::vec4(1,0,0,alpha);
    CALL_GL(glDrawArrays(GL_POINTS,0,1));
    res->flatShader["color"] = glm::vec4(0,1,0,alpha);
    CALL_GL(glDrawArrays(GL_POINTS,1,1));
    res->flatShader["color"] = glm::vec4(0,0,1,alpha);
    CALL_GL(glDrawArrays(GL_POINTS,2,1));
    res->flatShader["color"] = glm::vec4(1,1,0,alpha);
    CALL_GL(glDrawArrays(GL_POINTS,3,1));
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
