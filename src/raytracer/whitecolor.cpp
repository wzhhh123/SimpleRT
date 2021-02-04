
#include "base/header.h"
#include "whitecolor.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"


dVec3 WhiteColor::Trace(int level, Ray r) {

	dVec3 point = r.origin;
	dVec3 dir = r.direction;

	bool found = false;
	IntersectPoint nearestHit;
	found = Geometrys::Instance()->Intersect(r, &nearestHit);

	if (found) {

		dVec3 nor = { 0,0,1 };
		return nearestHit.tangentToWorld * nor;

		return nearestHit.normalWS;

		glm::vec4 diffuse = Renderer::Instance()->GetDiffuse(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 ambient = Renderer::Instance()->GetAmbient(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 emissive = Renderer::Instance()->GetEmissive(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 shininess = Renderer::Instance()->GetShininess(nearestHit.modelIndex, nearestHit.meshIndex);

		//std::cout << nearestHit.uv.x << " " << nearestHit.uv.y << std::endl;
//		return { nearestHit.uv.x, nearestHit.uv.y, 0 };
		//if(nearestHit.meshIndex!= 7)
		//return { nearestHit.meshIndex / 8.0 ,nearestHit.meshIndex / 8.0 ,nearestHit.meshIndex / 8.0 };
		//else {
		//	return { 1,0,1 };
		//}


		return shininess;
		return emissive;
		return ambient;
		return diffuse;
		return { diffuse.x, diffuse.y, diffuse.z };

		return { nearestHit.t / 5.0, nearestHit.t / 5.0 ,nearestHit.t / 5.0 };

		//return { 1,1,1 };
		//return nearestHit.normalOS;

		//return nearestHit.normalWS;
		//return nearestHit.normalTS;

	}
	else {
		return { 0,0,0 };
	}

}
