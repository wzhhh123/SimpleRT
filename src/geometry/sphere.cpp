
#include "sphere.h"



bool Sphere::Intersect(Ray r, FLOAT* t)
{
	FLOAT tmin = 1e30;

	glm::vec3 rayToCenter = center - r.origin;
	FLOAT b = glm::dot(rayToCenter, r.direction);
	FLOAT delta = b * b - glm::dot(rayToCenter, rayToCenter) + radius * radius;
	FLOAT res = delta > 0 ? sqrt(delta) : 1e31;
	res = b - res > 1e-7 ? b - res : b + res;
	if (res >= 1e-7 && res < tmin) {
		*t = res;
		return true;
	}
	return false;
}

