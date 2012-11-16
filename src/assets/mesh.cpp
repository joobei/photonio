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

	//calculate normals
	for (std::vector<GLushort>::size_type i=0; i != indices.size(); i+=3) {
		
		glm::vec3 v0,v1,v2;
		v0 = vertices[indices[i]];
		v1 = vertices[indices[i+1]];
		v2 = vertices[indices[i+2]];

		glm::vec3 U,V;
		
		U = v1 - v0;
		V = v2 - v0;

		normals.push_back(glm::normalize(glm::cross(U,V)));
		normals.push_back(glm::normalize(glm::cross(U,V)));
		normals.push_back(glm::normalize(glm::cross(U,V)));
	}


	farthestVertex = vertices[indices[0]];
	GLushort farthestIndex=0;

	for (std::vector<GLushort>::size_type i=0; i != indices.size(); i+=3) {
		
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

	//find farthest vertex and save for sphere radius
	for (std::vector<glm::vec3>::size_type i=0; i!= vertices.size(); i++) {

		if(glm::distance(vertices[i],glm::vec3(0,0,0)) > glm::distance(vertices[farthestIndex],glm::vec3(0,0,0))) {
			farthestVertex = vertices[farthestIndex];
			farthestIndex = i;
		}
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
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,normalVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),normals.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(normalLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(normalLoc));

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
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,normalVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),normals.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(normalLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(normalLoc));

	CALL_GL(glBindVertexArray(0));

	glm::vec3 center = glm::vec3(0,0,0);

	CALL_GL(glGenVertexArrays(1,&circlevaoId)); 
	CALL_GL(glGenBuffers(1,&circleVboId));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,circleVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,3*sizeof(GLfloat),glm::value_ptr(center),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindVertexArray(0));
}

//loads a simple mesh (like ray and plane)
pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> colorz, pho::PrimitiveType type):
vertices(vertixes),
	indices(indixes),
	colors(colorz)
{
	switch (type) {
	case Ray:
		wfindices.push_back(indixes[0]);
		wfindices.push_back(indixes[1]);
		break;
	case Plane:
		wfindices.push_back(indixes[0]);
		wfindices.push_back(indixes[1]);
		wfindices.push_back(indixes[2]);
		wfindices.push_back(indixes[3]);
		wfindices.push_back(indixes[4]);
		wfindices.push_back(indixes[5]);
		wfindices.push_back(indixes[6]);
		wfindices.push_back(indixes[7]);
		break;
	case Point:
		break;
	}

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
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
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
	return glm::vec3(modelMatrix[3]);
}

void pho::Mesh::drawCircle() {
	
	CALL_GL(glBindVertexArray(circlevaoId));
	CALL_GL(glDrawArrays(GL_POINTS,0,1));
	
	//CALL_GL(glBindVertexArray(vaoId));
	//CALL_GL(glDrawArrays(GL_POINTS,0,1));
}

void pho::Mesh::drawPoint() {
	
	CALL_GL(glBindVertexArray(vaoId));
	CALL_GL(glDrawArrays(GL_POINTS,0,1));
}

void pho::Mesh::draw() {
	CALL_GL(glBindVertexArray(vaoId));
	CALL_GL(glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,NULL));
}

//draw wireframe (to be improved)
void pho::Mesh::draw(bool wireframe) {

	CALL_GL(glBindVertexArray(wfvaoId));
	CALL_GL(glDrawElements(GL_LINES,wfindices.size(),GL_UNSIGNED_SHORT,NULL));
}


//for a given ray, find out if that ray hits the mesh then return the closest hit point
bool pho::Mesh::findIntersection(glm::mat4 rayMatrix, glm::vec3& foundPoint) {
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;

	double epsilon = 0.000001;

	rayOrigin = glm::vec3(rayMatrix[3]);  //pick up position of ray from the matrix
	rayDirection = glm::mat3(rayMatrix)*glm::vec3(0,0,-1);  //direction of ray
	rayDirection = glm::normalize(rayDirection);

	glm::vec4 v0,v1,v2; //three vertices of the triangle to be tested

	//iterate through all faces
	for (std::vector<Face>::size_type i=0;i < faces.size(); i++) {

		v0 = glm::vec4(vertices[faces[i].a],1.0f); //vertices are kept in another vector object
		v1 = glm::vec4(vertices[faces[i].b],1.0f); //I fetch them using indices
		v2 = glm::vec4(vertices[faces[i].c],1.0f);

		v0 = modelMatrix*v0;  //Transform the vertex from model space ---> world space
		v1 = modelMatrix*v1;
		v2 = modelMatrix*v2;
		
		//currently disabled because glm says barycentric but they are not barycentric
		/*if there's a hit
		if (glm::intersectRayTriangle(rayOrigin,rayDirection,glm::vec3(v0),glm::vec3(v1),glm::vec3(v2),intersection)) {
			static int count  = 0;

			//convert intersection point from barycentric to cartesian
			//currently does not work because of glm::intersectRayTriangle bug
			foundPoint = intersection;
		
			std::cout << count << " - Found x: \t" <<foundPoint.x << " \t" << foundPoint.y << " \t" << foundPoint.z << '\n'; 
			count++;
			return true;
		}*/

		if (rayTriangleIntersection(glm::vec3(v0),glm::vec3(v1),glm::vec3(v2),rayMatrix,(float)epsilon,foundPoint)) {
			return true;

		}

	}
	return false;

}

bool pho::Mesh::findIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& foundPoint) {
	

	double epsilon = 0.000001;

	glm::vec4 v0,v1,v2; //three vertices of the triangle to be tested

	//iterate through all faces
	for (std::vector<Face>::size_type i=0;i < faces.size(); i++) {

		v0 = glm::vec4(vertices[faces[i].a],1.0f); //vertices are kept in another vector object
		v1 = glm::vec4(vertices[faces[i].b],1.0f); //I fetch them using indices
		v2 = glm::vec4(vertices[faces[i].c],1.0f);

		v0 = modelMatrix*v0;  //Transform the vertex from model space ---> world space
		v1 = modelMatrix*v1;
		v2 = modelMatrix*v2;
		

		if (rayTriangleIntersection(glm::vec3(v0),glm::vec3(v1),glm::vec3(v2),rayOrigin, rayDirection,(float)epsilon,foundPoint)) {
			return true;

		}

	}
	return false;

}

bool pho::Mesh::findSphereIntersection(glm::mat4 rayMatrix,glm::vec3& foundPoint,float& foundDistance,glm::vec3& foundNormal) {
	glm::vec3 rayOrigin;
	glm::vec3 rayDirection;
	glm::vec3 sphereOrigin;
	float radius;

	rayOrigin = glm::vec3(rayMatrix[3]);  //pick up position of ray from the modelmatrix
	rayDirection = glm::mat3(rayMatrix)*glm::vec3(0,0,-1);  //direction of ray
	rayDirection = glm::normalize(rayDirection);
	sphereOrigin = glm::vec3(modelMatrix[3]);
	radius = glm::length(farthestVertex); //since the farthest vertex is from the origin 0,0,0 then lehgth() should just give us the sphere radius

	if (raySphereIntersection(rayDirection,rayOrigin,sphereOrigin,radius,foundPoint,foundDistance,foundNormal))
	{
		return true;	
	} else return false;
}

bool pho::Mesh::findSphereIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& foundPoint,float& foundDistance,glm::vec3& foundNormal) {
	
	glm::vec3 sphereOrigin;
	float radius;
	sphereOrigin = glm::vec3(modelMatrix[3]);
	radius = glm::length(farthestVertex); //since the farthest vertex is from the origin 0,0,0 then lehgth() should just give us the sphere radius

	if (raySphereIntersection(rayDirection,rayOrigin,sphereOrigin,radius,foundPoint,foundDistance,foundNormal))
	{
		return true;	
	} else return false;
}

bool pho::Mesh::rayTriangleIntersection(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::mat4 rayMatrix, float epsilon, glm::vec3 &intersection) {

	glm::vec3 lineDirection = glm::mat3(rayMatrix)*glm::vec3(0,0,-1);  //direction of ray
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

bool pho::Mesh::rayTriangleIntersection(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 rayOrigin, glm::vec3 rayDirection, float epsilon, glm::vec3 &intersection) {

	glm::vec3 lineDirection = rayDirection;
	glm::vec3 lineOrigin = rayOrigin;

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



inline float pho::Mesh::sum(const vec3& v)
{
return v[0] + v[1] + v[2];
}

bool pho::Mesh::raySphereIntersection(const vec3& raydir, const vec3& rayorig,const vec3& pos,const float& rad, vec3& hitpoint,float& distance, vec3& normal)
{
	float a = sum(raydir*raydir);
	float b = sum(raydir * (2.0f * ( rayorig - pos)));
	float c = sum(pos*pos) + sum(rayorig*rayorig) -2.0f*sum(rayorig*pos) - ARCBALL_RADIUS*ARCBALL_RADIUS;
	float D = b*b + (-4.0f)*a*c;

	// If ray can not intersect then stop
	if (D < 0)
		return false;
	D=sqrtf(D);

	// Ray can intersect the sphere, solve the closer hitpoint
	float t = (-0.5f)*(b+D)/a;
	if (t > 0.0f)
	{
		distance=sqrtf(a)*t;
		hitpoint=rayorig + t*raydir;
		normal=(hitpoint - pos) / ARCBALL_RADIUS;
	}
	else
		return false;
	return true;
}

void pho::Mesh::rotate(glm::mat4 rotationMatrix) {
	glm::vec4 tempPosition = modelMatrix[3];

	modelMatrix = rotationMatrix*modelMatrix;

	modelMatrix[3] = tempPosition;

}

void pho::Mesh::setPosition(const glm::vec3 &position) {
	modelMatrix[3] = glm::vec4(position,1.0f);
}