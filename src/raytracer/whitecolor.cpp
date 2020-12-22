
#include "base/header.h"
#include "whitecolor.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"



VEC3 WhiteColor::Trace(int level, Ray r) {

	VEC3 point = r.origin;
	VEC3 dir = r.direction;

	bool found = false;
	FLOAT mint = 1e30;
	int index = 0;
	found = Geometrys::Instance()->Intersect(r, &mint, &index);

	if (found) {
		return { 1,1,1 };
	}
	else {
		return { 0,0,0 };
	}

}
