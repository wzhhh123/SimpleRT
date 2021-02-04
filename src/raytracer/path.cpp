
#include "base/header.h"
#include "path.h"
#include "base/shape.h"
#include "base/geometrys.h"
#include "geometry/sphere.h"
#include "base/renderer.h"
#include "pcg32.h"



dVec3 Path::Trace(int level, Ray r) {

	dVec3 L(0), beta(1.0);
	bool specularBounce = false;

	int bounds = 0;
	FLOAT etaScale = 1;

	pcg32& rng = Renderer::Instance()->rng;

	for (bounds;; ++bounds) {

		bool found = false;
		IntersectPoint nearestHit;
		found = Geometrys::Instance()->Intersect(r, &nearestHit);

		if (!found || bounds >= level) break;

		if (bounds == 0 || specularBounce) {
			L += beta * nearestHit.Le(-r.direction);
			if (Renderer::Instance()->models[nearestHit.modelIndex]->meshes[nearestHit.meshIndex].isAreaLight) {
				if (bounds == 0) {
					//L = dVec3(1, 0, 1);
				}
				//L = { 1,1,1 };
				break; //灯
			}
		}

		if (Renderer::Instance()->models[nearestHit.modelIndex]->meshes[nearestHit.meshIndex].isAreaLight) {
			break;
		}

		//TODO sample one light
		{
			L += beta * UniformSampleOneLight(rng, nearestHit, r);
		}


		//*L = nearestHit.tangentToWorld * dVec3{ 0,0,1 };
	/*	L = nearestHit.normalWS;
		break;*/

		FLOAT pdf;
		dVec3 wo = -r.direction, wi;

		BxDF* bxdf = Renderer::Instance()->GetBxDF(nearestHit.modelIndex, nearestHit.meshIndex);
		dVec3 f = bxdf->Sample_f(glm::normalize(nearestHit.worldToTangent * wo), &wi, { rng.nextFloat(), rng.nextFloat() }, &pdf);

	//	std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		if (f.x == 0.0 &&f.y == 0.0 &&f.z == 0.0) break;
		if (pdf == 0.f) break;//不用追踪下去了，这条路径没用了

		beta *= f * std::abs(glm::dot( nearestHit.tangentToWorld * glm::normalize(wi)
			, nearestHit.normalWS)) / pdf;

		//dVec3 dir = nearestHit.tangentToWorld * glm::normalize(wi);
		//L = glm::dot(dir, nearestHit.normalWS) > 0 ? dVec3{1, 1, 1} : dVec3{0, 0, 0};
		//L = nearestHit.worldToTangent * nearestHit.normalWS;

		//创建新射线 这里可能有误差
		r.origin = nearestHit.t * r.direction + r.origin;
		r.direction = nearestHit.tangentToWorld * glm::normalize(wi);

		if (nearestHit.meshIndex == 7)break; //灯
		//这里可以加上rr
	}


	return L;


}

dVec3 Path::UniformSampleOneLight(pcg32& rng, IntersectPoint& point, Ray& r)
{
	FLOAT lightPdf = 0;
	int index = Geometrys::Instance()->lightDistribute.SampleDiscrete(rng.nextDouble(), &lightPdf);
	Triangle* triangle = dynamic_cast<Triangle*>(Geometrys::Instance()->shapes[Geometrys::Instance()->lightShapeIndices[index]]);
	FLOAT lightAreaPdf = 0;
	IntersectPoint it = triangle->Samping(dVec2{ rng.nextDouble(), rng.nextDouble() }, &lightAreaPdf);

	Ray shadowRay;
	shadowRay.origin = point.t * r.direction + r.origin;
	dVec3 lightPos = it.weightV * triangle->v0.vertexWS + it.weightU * triangle->v1.vertexWS + (1 - it.weightU - it.weightV) * triangle->v2.vertexWS;

	shadowRay.direction = glm::normalize(lightPos - shadowRay.origin);

	IntersectPoint nearestHit;
	bool found = Geometrys::Instance()->Intersect(shadowRay, &nearestHit);
	if (found) {
		dVec3 col = nearestHit.Le(-shadowRay.direction);

		dVec3 f = Renderer::Instance()->GetBxDF(point.modelIndex, point.meshIndex)->F(shadowRay.direction, shadowRay.direction);
		return col * f * std::abs(glm::dot(shadowRay.direction, point.normalWS)) / (lightPdf * lightAreaPdf);
		return col;// / (lightPdf * lightAreaPdf);
	}
	else {
		return dVec3(0, 0, 0);
	}
}
