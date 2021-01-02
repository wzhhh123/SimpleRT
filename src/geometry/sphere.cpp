
#include "sphere.h"



bool Sphere::Intersect(Ray r, IntersectPoint& p)
{
	FLOAT tmin = 1e30;

	dVec3 rayToCenter = center - r.origin;
	FLOAT b = glm::dot(rayToCenter, r.direction);
	FLOAT delta = b * b - glm::dot(rayToCenter, rayToCenter) + radius * radius;
	FLOAT res = delta > 0 ? sqrt(delta) : 1e31;
	res = b - res > 1e-7 ? b - res : b + res;
	if (res >= 1e-7 && res < tmin) {
		p.t = res;
		
		return true;
	}
	return false;
}


Sphere::Sphere(dVec3 _center, dVec3 _color, FLOAT _radius,
	FLOAT _kd, FLOAT _ks, FLOAT _kt, FLOAT _kl, FLOAT _ir):
	center(_center), color(_color),
	radius(_radius), kd(_kd), ks(_ks),
	kt(_kt), kl(_kl), ir(_ir) {
}


Sphere::Sphere() {}
