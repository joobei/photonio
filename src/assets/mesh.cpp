#pragma warning(disable: 4819)
#include "mesh.h"

pho::Mesh::Mesh() {
	modelMatrix = glm::mat4();
	selected = false;
}

//loads data and uploads to GPU
pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz):
vertices(vertixes),
	indices(indixes),
	colors(colorz)
{
	simple = false;

	for (std::vector<GLushort>::size_type i=0; i != indixes.size(); i+=3) {
		Face temp;
		temp.a = indixes[i];
		temp.b = indixes[i+1];
		temp.c = indixes[i+2];
		faces.push_back(temp);
		
		//for every face create indices to draw lines between every vertex
		wfindices.push_back(indixes[i]);
		wfindices.push_back(indixes[i+1]);
		wfindices.push_back(indixes[i+1]);
		wfindices.push_back(indixes[i+2]);
		wfindices.push_back(indixes[i+2]);
		wfindices.push_back(indixes[i]);
	}


	modelMatrix = glm::mat4();
	CALL_GL(glGenVertexArrays(1,&vaoId));
	CALL_GL(glGenBuffers(1,&ibId));
	CALL_GL(glGenBuffers(1,&vertexVboId));
	CALL_GL(glGenBuffers(1,&colorVboId));

	CALL_GL(glBindVertexArray(vaoId));

	

	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId));
	CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,colorVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*2*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindVertexArray(0));


	CALL_GL(glGenVertexArrays(1,&wfvaoId));  //vao for wireframe
	CALL_GL(glGenBuffers(1,&wfibId));

	CALL_GL(glBindVertexArray(wfvaoId));

	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,wfibId));
	CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,wfindices.size()*sizeof(GLushort),wfindices.data(),GL_STATIC_DRAW));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,colorVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*2*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindVertexArray(0));
}

//loads a simple mesh (like ray and plane)
pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz, bool simplex):
vertices(vertixes),
	indices(indixes),
	colors(colorz),
	simple(simplex)
{
	wfindices.push_back(indixes[0]);
	wfindices.push_back(indixes[1]);

	modelMatrix = glm::mat4();
	CALL_GL(glGenVertexArrays(1,&vaoId));
	CALL_GL(glGenBuffers(1,&ibId));
	CALL_GL(glGenBuffers(1,&vertexVboId));
	CALL_GL(glGenBuffers(1,&colorVboId));

	CALL_GL(glBindVertexArray(vaoId));
	
	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,wfibId));
	CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,wfindices.size()*sizeof(GLushort),wfindices.data(),GL_STATIC_DRAW));

	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibId));
	CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,colorVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*2*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindVertexArray(0));

	//for the wireframe
	CALL_GL(glGenVertexArrays(1,&wfvaoId));
	CALL_GL(glGenBuffers(1,&wfibId));

	CALL_GL(glBindVertexArray(wfvaoId));
	
	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,wfibId));
	CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,wfindices.size()*sizeof(GLushort),wfindices.data(),GL_STATIC_DRAW));

	
	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,colorVboId));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindVertexArray(0));


}


void pho::Mesh::setShader(pho::Shader* tehShader) {
	shader = tehShader;
}

bool pho::Mesh::loadToGPU() {

	GLuint buffer;  //buffer used to upload stuff to GPU memory

	//Generate Vertex Array for mesh
	CALL_GL(glGenVertexArrays(1,&(vaoId)));
	CALL_GL(glBindVertexArray(vaoId));

	//faces
	CALL_GL(glGenBuffers(1,&buffer));
	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
	//CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numFaces * 3, faces, GL_STATIC_DRAW));

	//vertices
	CALL_GL(glGenBuffers(1,&buffer));
	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertices, vertices.data(), GL_STATIC_DRAW));
	CALL_GL(glEnableVertexAttribArray(pho::vertexLoc));
	CALL_GL(glVertexAttribPointer(pho::vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0));

	//normals
	CALL_GL(glGenBuffers(1, &buffer));
	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*numVertices, normals.data(), GL_STATIC_DRAW));
	CALL_GL(glEnableVertexAttribArray(pho::normalLoc));
	CALL_GL(glVertexAttribPointer(pho::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0));

	//texture coordinates
	CALL_GL(glGenBuffers(1, &buffer));
	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*numVertices, texCoords, GL_STATIC_DRAW));
	CALL_GL(glEnableVertexAttribArray(pho::texCoordLoc));
	CALL_GL(glVertexAttribPointer(pho::texCoordLoc, 2, GL_FLOAT, 0, 0, 0));

	// unbind buffers
	CALL_GL(glBindVertexArray(0));
	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,0));
	CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));

	return true;
}

GLuint pho::Mesh::getVaoId() {
	return vaoId;
}

glm::vec3 pho::Mesh::getPosition() {
	return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}

void pho::Mesh::draw() {
	
	CALL_GL(glBindVertexArray(vaoId));
	CALL_GL(glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,NULL));
}

//draw wireframe (to be improved)
void pho::Mesh::draw(bool wireframe) {

	CALL_GL(glBindVertexArray(wfvaoId));
	CALL_GL(glLineWidth(2.5));
	CALL_GL(glDrawElements(GL_LINES,wfindices.size(),GL_UNSIGNED_SHORT,NULL));
}


//for a given ray, find out if that ray hits the mesh then return the closest hit point
bool pho::Mesh::findIntersection(glm::mat4 rayMatrix, glm::vec3& foundPoint) {
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;

	rayOrigin = glm::vec3(rayMatrix[3]);  //pick up position of ray from the matrix
	rayDirection = glm::mat3(rayMatrix)*glm::vec3(0,0,-1);  //direction of ray
	rayDirection = glm::normalize(rayDirection);

	glm::vec4 v0,v1,v2; //three vertices of the triangle to be tested

	//iterate through all faces
	for (std::vector<Face>::size_type i=0;i < faces.size(); i++) {
		glm::vec3 intersection;

		v0 = glm::vec4(vertices[faces[i].a],1.0f); //vertices are kept in another vector object
		v1 = glm::vec4(vertices[faces[i].b],1.0f); //I fetch them using indices
		v2 = glm::vec4(vertices[faces[i].c],1.0f);

		v0 = modelMatrix*v0;  //Transform the vertex from model space ---> world space
		v1 = modelMatrix*v1;
		v2 = modelMatrix*v2;
		
		//if there's a hit
		if (glm::intersectRayTriangle(rayOrigin,rayDirection,glm::vec3(v0),glm::vec3(v1),glm::vec3(v2),intersection)) {
			static int count  = 0;

			//convert intersection point from barycentric to cartesian
			foundPoint = intersection.x*glm::vec3(v0)+intersection.y*glm::vec3(v1)+intersection.z*glm::vec3(v2);
		
			std::cout << count << " - Found x: \t" <<foundPoint.x << " \t" << foundPoint.y << " \t" << foundPoint.z << '\n'; 
			count++;
			return true;
		}
	}
	return false;

}