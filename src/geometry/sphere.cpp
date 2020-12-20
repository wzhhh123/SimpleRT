
#include "sphere.h"



bool Sphere::Intersect(Ray r, FLOAT* t)
{
	FLOAT tmin = 1e30;

	glm::dvec3 rayToCenter = center - r.origin;
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


Sphere::Sphere(glm::dvec3 _center, glm::dvec3 _color, FLOAT _radius,
	FLOAT _kd, FLOAT _ks, FLOAT _kt, FLOAT _kl, FLOAT _ir):
	center(_center), color(_color),
	radius(_radius), kd(_kd), ks(_ks),
	kt(_kt), kl(_kl), ir(_ir) {
}


Sphere::Sphere() {}
