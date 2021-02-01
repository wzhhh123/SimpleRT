
#include "base/header.h"
#include "path.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"
#include "pcg32.h"



dVec3 Path::UniformSampleOneLight(pcg32& rng) {

}

dVec3 Path::Trace(int level, Ray r) {


	dVec3 L(0), beta(1.0);
	bool specularBounce = true;

	int bounds = 0;
	FLOAT etaScale = 1;

	pcg32& rng = Renderer::Instance()->rng;

	for (bounds;; ++bounds) {

		bool found = false;
		IntersectPoint nearestHit;
		found = Geometrys::Instance()->Intersect(r, &nearestHit);


		if (bounds == 0 || specularBounce) {
			L += beta * nearestHit.Le(-r.direction);
			if (nearestHit.meshIndex == 7) {
				if (bounds == 0) {
					//L = dVec3(1, 0, 1);
				}
				//L = { 1,1,1 };
				break; //灯
			}
		}

		if (!found || bounds >= level) break;

		//TODO sample one light
		{
			L += UniformSampleOneLight(rng);
		}



		FLOAT pdf;
		dVec3 wo = -r.direction, wi;

		Lambert* lam = dynamic_cast<Lambert*>(Renderer::Instance()->lambert);
		lam->albedo = Renderer::Instance()->GetDiffuse(nearestHit.modelIndex, nearestHit.meshIndex);
		dVec3 f = lam->Sample_f(wo, &wi, { rng.nextFloat(), rng.nextFloat() }, &pdf);

	//	std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		if (f.x == 0.0 &&f.y == 0.0 &&f.z == 0.0) break;
		if (pdf == 0.f) break;//不用追踪下去了，这条路径没用了

		beta *= f * std::abs(glm::dot( nearestHit.tangentToWorld * glm::normalize(wi)
			, nearestHit.normalWS)) / pdf;

		//创建新射线 这里可能有误差
		r.origin = nearestHit.t * r.direction + r.origin;
		r.direction = nearestHit.tangentToWorld * glm::normalize(wi);


		if (nearestHit.meshIndex == 7)break; //灯

		//这里可以加上rr
	}



	return L;


}
