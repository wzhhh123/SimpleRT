
#include "base/header.h"
#include "whitecolor.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"



dVec3 WhiteColor::Trace(int level, Ray r) {

	dVec3 point = r.origin;
	dVec3 dir = r.direction;

	bool found = false;
	IntersectPoint nearestHit;
	found = Geometrys::Instance()->Intersect(r, &nearestHit);

	if (found) {
		return { 1,1,1 };
		//return nearestHit.normalWS;
		//return nearestHit.normalOS;

		//return nearestHit.normalWS;
		//return nearestHit.normalTS;

	}
	else {
		return { 0,0,0 };
	}

}
