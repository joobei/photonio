#pragma warning(disable: 4819)
#include "mesh.h"

pho::Mesh::Mesh() {
	modelMatrix = glm::mat4();
	selected = false;
}

pho::Mesh::Mesh(std::vector<glm::vec3> vertixes) : vertices(vertixes)
{
	
	modelMatrix = glm::mat4();
	CALL_GL(glGenVertexArrays(1,&vaoId));	
	CALL_GL(glGenBuffers(1,&vertexVboId));


	CALL_GL(glBindVertexArray(vaoId));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindVertexArray(0));
}

//loads data and uploads to GPU
pho::Mesh::Mesh(std::vector<glm::vec3> vertixes, std::vector<glm::vec3> colorz):
vertices(vertixes),
	colors(colorz)
{
	if ((vertices.size() % 3) == 0 ) {
		//calculate normals
		for (std::vector<GLushort>::size_type i=0; i < vertices.size(); i+=3) {
			glm::vec3 v0,v1,v2;
			v0 = vertices[i];
			v1 = vertices[i+1];
			v2 = vertices[i+2];

			glm::vec3 U,V;

			U = v1 - v0;
			V = v2 - v0;

			Face temp;
			temp.a = v0;
			temp.b = v1;
			temp.c = v2;
			faces.push_back(temp);
			normals.push_back(glm::normalize(glm::cross(U,V)));
			normals.push_back(glm::normalize(glm::cross(U,V)));
			normals.push_back(glm::normalize(glm::cross(U,V)));
		}
	}

	modelMatrix = glm::mat4();
	CALL_GL(glGenVertexArrays(1,&vaoId));
	
	CALL_GL(glGenBuffers(1,&normalVboId));
	CALL_GL(glGenBuffers(1,&vertexVboId));
	CALL_GL(glGenBuffers(1,&colorVboId));

	CALL_GL(glBindVertexArray(vaoId));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,vertexVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(vertexLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,colorVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(colorLoc));

	CALL_GL(glBindBuffer(GL_ARRAY_BUFFER,normalVboId));
	CALL_GL(glBufferData(GL_ARRAY_BUFFER,normals.size()*3*sizeof(GLfloat),normals.data(),GL_STATIC_DRAW));
	CALL_GL(glVertexAttribPointer(normalLoc,3,GL_FLOAT,GL_FALSE,0,0));
	CALL_GL(glEnableVertexAttribArray(normalLoc));

	CALL_GL(glBindVertexArray(0));

}


void pho::Mesh::setShader(pho::Shader* tehShader) {
	shader = tehShader;
}

GLuint pho::Mesh::getVaoId() {
	return vaoId;
}

glm::vec3 pho::Mesh::getPosition() {
	return glm::vec3(modelMatrix[3]);
}

void pho::Mesh::bind() {
	CALL_GL(glBindVertexArray(vaoId));
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

		v0 = glm::vec4(faces[i].a,1.0f); //vertices are kept in another vector object
		v1 = glm::vec4(faces[i].b,1.0f); //I fetch them using indices
		v2 = glm::vec4(faces[i].c,1.0f);

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

		v0 = glm::vec4(faces[i].a,1.0f); //vertices are kept in another vector object
		v1 = glm::vec4(faces[i].b,1.0f); //I fetch them using indices
		v2 = glm::vec4(faces[i].c,1.0f);

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