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

	for (std::vector<GLushort>::size_type i=0; i != indixes.size(); i+=3) {
		Face temp;
		temp.a = indixes[i];
		temp.b = indixes[i+1];
		temp.c = indixes[i+2];
		faces.push_back(temp);
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

}

pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz, bool simple):
vertices(vertixes),
	indices(indixes),
	colors(colorz)
{
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

void pho::Mesh::drawLines() {
	CALL_GL(glBindVertexArray(vaoId));
	CALL_GL(glLineWidth(3.5));
	CALL_GL(glDrawElements(GL_LINES,indices.size(),GL_UNSIGNED_SHORT,NULL));
	CALL_GL(glLineWidth(1.0));
}


//for a given ray, find out if that ray hits the mesh then return the closest hit point
bool pho::Mesh::findIntersection(glm::mat4 rayMatrix, glm::vec3& foundPoint) {
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;

	rayOrigin = glm::vec3(rayMatrix[3]);  //pick up position of ray from the matrix
	rayDirection = glm::vec3(0,0,-1)*glm::mat3(rayMatrix);  //direction of ray

	glm::vec3 v0,v1,v2; //three vertices of the triangle to be tested
	std::vector<glm::vec3> foundPoints;

	float epsilon = 0.000001f;

	//iterate through all faces
	for (std::vector<Face>::size_type i=0;i < faces.size(); i++) {
		glm::vec3 intersection;

		v0 = vertices[faces[i].a]; //vertices are kept in another vector object
		v1 = vertices[faces[i].b]; //I fetch them using indices
		v2 = vertices[faces[i].c];

		v0 = glm::mat3(modelMatrix)*v0;  //first rotate the vertex
		v0 = v0+glm::vec3(modelMatrix[3]); //then translate it

		v1 = glm::mat3(modelMatrix)*v1;
		v1 = v1+glm::vec3(modelMatrix[3]);

		v2 = glm::mat3(modelMatrix)*v2;
		v2 = v2+glm::vec3(modelMatrix[3]);


		if (glm::intersectLineTriangle(rayOrigin,rayDirection,v0,v1,v2,intersection)) {


			foundPoint = intersection;

			return true;

		}
	}
	return false;

}

bool pho::Mesh::lineTriangleIntersection(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::mat4 rayMatrix, float epsilon, glm::vec3 &intersection) {

	glm::vec3 lineDirection = glm::vec3(0,0,-1)*glm::mat3(rayMatrix);  //direction of ray
	glm::vec3 lineOrigin = glm::vec3(rayMatrix[3][0],rayMatrix[3][1],rayMatrix[3][2]);

	glm::vec3 e1,e2,p,s,q;

	float t,u,v,tmp;

	e1 = v1-v0;
	e2 = v2-v0;
	p=glm::cross(lineDirection,e2);
	tmp = glm::dot(p,e1);

	if((tmp > -epsilon) && (tmp < epsilon)) {
		return false;
	}

	tmp = 1.0f/tmp;
	s = lineOrigin - v0;

	u=tmp*glm::dot(s,p);
	if (u < 00 || u > 1.0) {
		return false;
	}

	q = glm::cross(s,e1);
	v = tmp * glm::dot(lineDirection,q);

	if(v<0.0||v>1.0) {
		return false;
	}

	if (u+v > 1.0) {
		return false;
	}

	t = tmp * glm::dot(e2,q);

	//info here (probably barycentric coordinates)

	//intersection point
	intersection = lineOrigin + t*lineDirection;
	return true;

}

bool pho::Mesh::RayIntersectTriangle(glm::mat4 rayMatrix, const glm::vec3 &T0, const glm::vec3 &T1, const glm::vec3 &T2, glm::vec3 &IntersectPt)
{
	glm::vec3 RayPt;
	glm::vec3 RayDir;
	RayDir = glm::vec3(0,0,-1)*glm::mat3(rayMatrix);  //direction of ray
	RayPt = glm::vec3(rayMatrix[3][0],rayMatrix[3][1],rayMatrix[3][2]);

	vec3 u, v, n;             // triangle vectors
	vec3 w0, w;               // ray vectors
	float r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T1 - T0;
	v = T2 - T0;
	n = glm::cross(u, v);

	w0 = RayPt - T0;
	a = -glm::dot(n, w0);
	b = glm::dot(n, RayDir);
	if(b == 0.0f)
	{
		return false;
	}
	/*if (fabsf(b) < SMALL_NUM) {  // ray is parallel to triangle plane
	if (a == 0)                // ray lies in triangle plane
	return 2;
	else return 0;             // ray disjoint from plane
	}*/

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0f)                  // ray goes away from triangle
	{
		return false;              // => no intersect
	}
	// for a segment, also test if (r > 1.0) => no intersect

	IntersectPt = RayPt + r * RayDir; // intersect point of ray and plane

	// is I inside T?
	float uu, uv, vv, wu, wv, D;
	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	w = IntersectPt - T0;
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = 1.0f / (uv * uv - uu * vv);

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0f || s > 1.0f)        // I is outside T
	{
		return false;
	}
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0f || (s + t) > 1.0f)  // I is outside T
	{
		return false;
	}
	return true;
}