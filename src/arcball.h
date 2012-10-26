#pragma warning(disable: 4819)

#ifndef ARCBALL_H
#define ARCBALL_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/matrix_interpolation.hpp"

using glm::vec3;
using glm::mat4;

namespace pho {

	class Arcball {
	public:
		Arcball(bool fullscreen);
		void setCenter(glm::vec3 center);
		void setRadius(float radius);
		void beginDrag(glm::vec3 startingPoint);
		mat4 getRotation(glm::vec3 newPoint);
		void endDrag(glm::vec3 endPoint);
		vec3 oldPoint;
	private:
		float sum(const vec3& v);
		bool raySphereIntersection(const vec3& raydir, const vec3& rayorig,const vec3& pos,const float& rad, vec3& hitpoint,float& distance, vec3& normal);
		
		vec3 center;
		bool fullScreen;
		float radius;
	};
}

#endif
