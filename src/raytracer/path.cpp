
#include "base/header.h"
#include "path.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"


dVec3 Path::Trace(int level, Ray r) {


	dVec3 L(0), beta(1.0);
	bool specularBounce = false;

	int bounds;
	FLOAT etaScale = 1;

	for (bounds;; ++bounds) {

		bool found = false;
		IntersectPoint nearestHit;
		found = Geometrys::Instance()->Intersect(r, &nearestHit);

		if (bounds == 0 || specularBounce) {
			L += beta * nearestHit.Le(-r.direction);
		}

		if (!found || bounds >= level) break;
		auto diffuse = Renderer::Instance()->GetDiffuse(nearestHit.modelIndex, nearestHit.meshIndex);
		dVec3 f = dVec3{ diffuse.x, diffuse.y, diffuse.z } * InvPi;
		
		beta *= f;


	}







	dVec3 point = r.origin;
	dVec3 dir = r.direction;

	bool found = false;
	IntersectPoint nearestHit;
	found = Geometrys::Instance()->Intersect(r, &nearestHit);

	if (found) {


		glm::vec4 diffuse = Renderer::Instance()->GetDiffuse(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 ambient = Renderer::Instance()->GetAmbient(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 emissive = Renderer::Instance()->GetEmissive(nearestHit.modelIndex, nearestHit.meshIndex);
		glm::vec4 shininess = Renderer::Instance()->GetShininess(nearestHit.modelIndex, nearestHit.meshIndex);
		if(nearestHit.meshIndex!= 7)
		return { nearestHit.meshIndex / 8.0 ,nearestHit.meshIndex / 8.0 ,nearestHit.meshIndex / 8.0 };
		else {
			return { 1,0,1 };
		}
		return shininess;
		return emissive;
		return ambient;
		return diffuse;
		return { diffuse.x, diffuse.y, diffuse.z };

		return { nearestHit.t / 5.0, nearestHit.t / 5.0 ,nearestHit.t / 5.0 };

		//return { 1,1,1 };
		return nearestHit.normalWS;
		//return nearestHit.normalOS;

		//return nearestHit.normalWS;
		//return nearestHit.normalTS;

	}
	else {
		return { 0,0,0 };
	}

}
