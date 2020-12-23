
#include "base/header.h"
#include "businesscard.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"



dVec3 BusinessCard::Trace(int level, Ray r) {

	dVec3 point = r.origin;
	dVec3 dir = r.direction;
	if (!level--)
		return { 0,0,0 };

	bool found = false;
	FLOAT mint = 1e30;
	int index = 0;
	found = Geometrys::Instance()->Intersect(r, &mint, &index);

	if (found) {
		;
	}
	else {
		return { 0,0,0 };
	}

	Sphere* s = dynamic_cast<Sphere*>(Geometrys::Instance()->shapes[index]);
	dVec3 color = { 0,0,0 };
	FLOAT eta = s->ir;
	dVec3 P = r.origin + r.direction * mint;
	dVec3 N = glm::normalize(s->center - P);
	FLOAT d = -glm::dot(r.direction, N);
	if (d < 0) {
		N = -N;
		eta = 1 / eta;
		d = -d;
	}

	for (size_t i = 0; i < Geometrys::Instance()->shapes.size(); ++i) {
		Sphere* sphere = dynamic_cast<Sphere*>(Geometrys::Instance()->shapes[i]);
		dVec3 U = glm::normalize(sphere->center - P);
		FLOAT e = sphere->kl * glm::dot(N, U);
		Ray reflectRay = { P, U };
		IntersectPoint intersectPoint;
		if (e > 0 && sphere->Intersect(reflectRay, intersectPoint)) {
			color += sphere->color * e;
		}
	}

	dVec3 intersectColor = s->color;
	color *= intersectColor;
	FLOAT temp = 1 - eta * eta*(1 - d * d);

	return (s->kt)*(temp > 0 ? Trace(level, { P, eta*r.direction + N * (eta*d - sqrt(temp)) }) : dVec3{ 0,0,0 }) + 
		(s->ks)*Trace(level, { P, 2 * d * N + r.direction }) + 
		s->kl*intersectColor + 
		s->kd* color;


}
