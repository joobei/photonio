#pragma warning(disable: 4819)

#include "mesh.h"

pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz):
vertices(vertixes),
	indices(indixes),
    colors(colorz)
{
	modelMatrix = glm::mat4();
	glGenVertexArrays(1,&vaoId);
	glGenBuffers(1,&ibId);
	glGenBuffers(1,&vertexVboId);
	glGenBuffers(1,&colorVboId);

	glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId);
	glBindBuffer(GL_ARRAY_BUFFER,vertexVboId);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW);
	glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER,colorVboId);
	glBufferData(GL_ARRAY_BUFFER,colors.size()*2*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(texCoordLoc,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(texCoordLoc);
	glBindVertexArray(0);

}

void pho::Mesh::setShader(pho::Shader* tehShader) {
    shader = tehShader;
}

bool pho::Mesh::loadToGPU() {

	GLuint buffer;  //buffer used to upload stuff to GPU memory
	
	//Generate Vertex Array for mesh
	glGenVertexArrays(1,&(vaoId));
	glBindVertexArray(vaoId);

	//faces
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numFaces * 3, faces, GL_STATIC_DRAW);

	//vertices
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertices, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(pho::vertexLoc);
	glVertexAttribPointer(pho::vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//normals
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertices, normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(pho::normalLoc);
	glVertexAttribPointer(pho::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//texture coordinates
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*numVertices, texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(pho::texCoordLoc);
	glVertexAttribPointer(pho::texCoordLoc, 2, GL_FLOAT, 0, 0, 0);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return true;
}

inline GLuint pho::Mesh::getVaoId() {
	return vaoId;
}

glm::vec3 pho::Mesh::getPosition() {
	return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}

void pho::Mesh::draw() {
    shader->use(); //bind the shader this mesh uses
    glBindBuffer(GL_ARRAY_BUFFER,vaoId); //bind the vao
    glDrawRangeElements(GL_TRIANGLES,0,vertices.size(),indices.size(),GL_UNSIGNED_SHORT,NULL);
}
