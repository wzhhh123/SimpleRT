
#include "base/header.h"
#include "path.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"




dVec3 Path::Trace(int level, Ray r) {

	dVec3 L(0), beta(1.0);
	bool specularBounce = false;

	int bounds = 0;
	FLOAT etaScale = 1;

	pcg32& rng = Renderer::Instance()->rng;
	float rrThreshold = 1;

	for (bounds;; ++bounds) {

		bool found = false;
		IntersectPoint nearestHit;
		found = Geometrys::Instance()->Intersect(r, &nearestHit);

		if (bounds == 0 || specularBounce) {
			if(found)
				L += beta * nearestHit.Le(-r.direction, nearestHit);
		}

		if (!found || bounds >= level) break;

		//if (Renderer::Instance()->models[nearestHit.modelIndex]->meshes[nearestHit.meshIndex].isAreaLight) break;

		//if() not specular
		{
			L += beta * UniformSampleOneLight(rng, nearestHit, r);
		}

		//*L = nearestHit.tangentToWorld * dVec3{ 0,0,1 };
		//L = nearestHit.normalWS;
		//break;

		FLOAT pdf;
		dVec3 wo = -r.direction, wi;

		BxDF* bxdf = Renderer::Instance()->GetBxDF(nearestHit.modelIndex, nearestHit.meshIndex);
		dVec3 f = bxdf->Sample_f(glm::normalize(nearestHit.worldToTangent * wo), &wi, { rng.nextFloat(), rng.nextFloat() }, &pdf);

	//	std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		if (f.x < 1e-6 && f.y < 1e-6 && f.z < 1e-6)
        {
#if DEBUG_MODE
            if(nearestHit.IsHitAreaLight())
            {
//                std::string str = "ray hit arealight and end trace!\n";
//                std::cout << str;
//                std::cout.flush();
            }
#endif
            break;
        }
		if (pdf == 0.f) break;//不用追踪下去了，这条路径没用了

		beta *= f * std::abs(glm::dot( nearestHit.tangentToWorld * glm::normalize(wi)
			, nearestHit.normalWS)) / pdf;

		//dVec3 dir = nearestHit.tangentToWorld * glm::normalize(wi);
		//L = glm::dot(dir, nearestHit.normalWS) > 0 ? dVec3{1, 1, 1} : dVec3{0, 0, 0};
		//L = nearestHit.worldToTangent * nearestHit.normalWS;

		r.origin = nearestHit.t * r.direction + r.origin;
		r.direction = nearestHit.tangentToWorld * glm::normalize(wi);

        //rr from pbrt
		dVec3 rrBeta = beta;
		FLOAT maxComponent = std::max(std::max(rrBeta.x, rrBeta.y), rrBeta.z);
		if ((maxComponent < rrThreshold) && bounds >= 3) {
			FLOAT q = std::max((FLOAT).05, 1 - maxComponent);
			if (rng.nextDouble() < q) break;
			beta /= 1 - q;
		}
	}

	return L;

}

