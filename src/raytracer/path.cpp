
#include "base/header.h"
#include "path.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"
#include "base/sampler.h"



dVec3 Path::Trace(int level, Ray r, std::shared_ptr<Sampler>sampler) {

	dVec3 L(0), beta(1.0);
	bool specularBounce = false;

	int bounds = 0;
	FLOAT etaScale = 1;

	float rrThreshold = 1;
    bool specularBound = false;
    for (bounds;; ++bounds) {

        bool found = false;
        IntersectPoint nearestHit;
        found = Geometrys::Instance()->Intersect(r, &nearestHit);

        //if (Renderer::Instance()->models[nearestHit.modelIndex]->meshes[nearestHit.meshIndex].isAreaLight) break;

        //BxDF* bxdf = Renderer::Instance()->GetBxDF(nearestHit.modelIndex, nearestHit.meshIndex);
        if (bounds == 0 || specularBound)
        {
            if (found)
            {
                L += beta * nearestHit.Le(-r.direction, nearestHit);;
            }
            else
            {

            }
        }
        if (!found || bounds >= level)
        {
            break;
        }
   

        nearestHit.ComputeScatteringFunctions(Renderer::Instance()->models[nearestHit.modelIndex]->meshes[nearestHit.meshIndex]);
        
		//*L = nearestHit.tangentToWorld * dVec3{ 0,0,1 };
		//L = nearestHit.normalWS;
		//break;
  
        //return glm::normalize(nearestHit.worldToTangent * nearestHit.normalWS);
        //return glm::normalize(nearestHit.tangentToWorld * dVec3(0,0,1));
        //return dVec3(1,1,1);
        //return {nearestHit.uv.x,nearestHit.uv.y,0};
        //return nearestHit.normalWS;
        
        
        if((nearestHit.GetBxDFType() & ~BSDF_SPECULAR) > 0)
        {
            L += beta * UniformSampleOneLight(sampler, nearestHit, r);
        }

        FLOAT pdf;
        dVec3 wo = -r.direction, wi;

        BxDFType type = (BxDFType)0;
        dVec3 f = nearestHit.bsdf->Sample_f(glm::normalize(nearestHit.worldToTangent * wo), &wi, sampler->Get2D(), &pdf, BSDF_ALL, nearestHit);

        beta *= f * std::abs(glm::dot(nearestHit.tangentToWorld * glm::normalize(wi)
            , nearestHit.normalWS)) / pdf;

        specularBound = (type & BSDF_SPECULAR) != 0;

        //    std::cout << f.x << " " << f.y << " " << f.z << std::endl;

        if ((f.x == 0.0 && f.y == 0.0 && f.z == 0.0) || pdf == 0.0)
        {
            break;
        }

        //beta *= f * std::abs(glm::dot(glm::normalize(wi), dVec3(0,0,1))) / pdf;

		//dVec3 dir = nearestHit.tangentToWorld * glm::normalize(wi);
		//L = glm::dot(dir, nearestHit.normalWS) > 0 ? dVec3{1, 1, 1} : dVec3{0, 0, 0};
		//L = nearestHit.worldToTangent * nearestHit.normalWS;

		r.origin = nearestHit.t * r.direction + r.origin;
        r.direction = glm::normalize(nearestHit.tangentToWorld * glm::normalize(wi));

        //rr from pbrt
		dVec3 rrBeta = beta;
		FLOAT maxComponent = std::max(std::max(rrBeta.x, rrBeta.y), rrBeta.z);
		if (bounds > 3 && (maxComponent < rrThreshold)) {
			FLOAT q = std::max((FLOAT).05, 1 - maxComponent);
			if (sampler->Get1D() < q) break;
			beta /= 1 - q;
		}
	}
    

	return L;

}

