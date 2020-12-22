
#include "triangle.h"




Triangle::Triangle(VEC3 _v0, VEC3 _v1, VEC3 _v2) : v0(_v0), v1(_v1), v2(_v2)
{

}


//克莱姆法则+向量混合积
bool Triangle::Intersect(Ray r, IntersectPoint& p)
{

	auto orig = r.origin;
	auto dir = r.direction;

	// E1
	VEC3 E1 = v1 - v0;

	// E2
	VEC3 E2 = v2 - v0;

	// P
	VEC3 P = glm::cross(dir, E2);

	// determinant
	FLOAT det = glm::dot(E1, P);

	VEC3 T;
	if (det > 0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	// Calculate u and make sure u <= 1
	p.u = glm::dot(T, P);
	if (p.u < 0.0f || p.u > det)
		return false;

	// Q
	VEC3 Q = glm::cross(T, E1);

	// Calculate v and make sure u + v <= 1
	p.v = glm::dot(dir, Q);
	if (p.v < 0.0f || p.u + p.v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	p.t = glm::dot(E2, Q);

	FLOAT fInvDet = 1.0f / det;
	p.t *= fInvDet;
	p.u *= fInvDet;
	p.v *= fInvDet;

	return true;

}
