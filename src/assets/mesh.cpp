#pragma warning(disable: 4819)

#include "mesh.h"

pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec2> texcoords,std::string name):
vertices(vertixes),
	indices(indixes),
	texcoords(texcoords),
	selected(false),
	name(name)
{
	modelMatrix = glm::mat4();
	glGenVertexArrays(1,&vaoId);
	glGenBuffers(1,&ibId);
	glGenBuffers(1,&vertexVboId);
	glGenBuffers(1,&texCoordVboId);

	glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId);
	glBindBuffer(GL_ARRAY_BUFFER,vertexVboId);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW);
	glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(vertexLoc);

	glBindBuffer(GL_ARRAY_BUFFER,texCoordVboId);
	glBufferData(GL_ARRAY_BUFFER,texcoords.size()*2*sizeof(GLfloat),texcoords.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(texCoordLoc,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(texCoordLoc);
	glBindVertexArray(0);
}

pho::Mesh::draw() {

	GLuint buffer;  //buffer used to upload stuff to GPU memory

	// generate Vertex Array for mesh
	glGenVertexArrays(1,&(vaoId));
	glBindVertexArray(vaoId);

	// buffer for faces
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numFaces * 3, faces, GL_STATIC_DRAW);


	// buffer for vertex positions
	if (mesh->HasPositions()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(pho::vertexLoc);
		glVertexAttribPointer(pho::vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// buffer for vertex normals
	if (mesh->HasNormals()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(pho::normalLoc);
		glVertexAttribPointer(pho::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//for (int i=0;i<100;i++) {
		//	std::cout << mesh->mNormals[i].x << " "; 
		//}
		//std::cout << '\n';
	}


	// buffer for vertex texture coordinates
	if (mesh->HasTextureCoords(0)) {
		float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
		for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

			texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
			texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

		}
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(pho::texCoordLoc);
		glVertexAttribPointer(pho::texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

inline GLuint pho::Mesh::getVaoId() {
	return vaoId;
}

glm::vec3 pho::Mesh::getPosition() {

	return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}
