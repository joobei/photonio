#pragma warning(disable: 4819)

#include "arcball.h"

pho::Arcball::Arcball(bool fullscreen): 
	fullScreen(fullscreen),
	center(glm::vec3(0)){ }

void pho::Arcball::setCenter(glm::vec3 centre) {
	center = centre;
}

void pho::Arcball::setRadius(float radiuse){
	radius = radiuse;
}

void pho::Arcball::beginDrag(glm::vec3 startingPoint){
	
	vec3 projectedNewPoint;
	vec3 normal;
	float distance;

	if (raySphereIntersection(vec3(0,0,-1),startingPoint,center,radius,projectedNewPoint,distance,normal)) {			
		oldPoint = projectedNewPoint;	
	}
}

glm::mat4 pho::Arcball::getRotation(glm::vec3 newPoint){
	vec3 projectedNewPoint;
	vec3 normal;
	float distance;

	if (raySphereIntersection(vec3(0,0,-1),newPoint,center,radius,projectedNewPoint,distance,normal)) {		
		vec3 axis,currentPoint;
		float angle;
		currentPoint = projectedNewPoint-center;

		axis = glm::cross(oldPoint,currentPoint);
		angle = acosf((glm::dot(oldPoint,currentPoint))/glm::length(oldPoint)*glm::length(currentPoint));

		oldPoint = projectedNewPoint;

		return glm::toMat4(glm::angleAxis(angle,axis));
	}
	else return glm::mat4(1);

}

void pho::Arcball::endDrag(glm::vec3 endPoint) {
	
}

inline float pho::Arcball::sum(const vec3& v)
{
	return v[0] + v[1] + v[2];
}

bool pho::Arcball::raySphereIntersection(const vec3& raydir, const vec3& rayorig,const vec3& pos,const float& rad, vec3& hitpoint,float& distance, vec3& normal)
{
	float a = sum(raydir*raydir);
	float b = sum(raydir * (2.0f * ( rayorig - pos)));
	float c = sum(pos*pos) + sum(rayorig*rayorig) -2.0f*sum(rayorig*pos) - rad*rad;
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
		normal=(hitpoint - pos) / rad;
	}
	else
		return false;
	return true;
}
