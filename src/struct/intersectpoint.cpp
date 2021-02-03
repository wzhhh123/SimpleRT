

#include "intersectpoint.h"
#include "base/renderer.h"


dVec3 IntersectPoint::Le(dVec3 direction) {


	if (Renderer::Instance()->models[modelIndex]->meshes[meshIndex].isAreaLight) {
	
		if (glm::dot(direction, normalWS) > 0) {
			return Renderer::Instance()->GetEmissive(modelIndex, meshIndex) * 2.0f;
		}

	}

	return { 0,0,0 };

}
