

#include "intersectpoint.h"
#include "base/renderer.h"


dVec3 IntersectPoint::Le(const dVec3& direction, const IntersectPoint& it) {


	if (Renderer::Instance()->models[modelIndex]->meshes[meshIndex].isAreaLight) {
        
        //debug code
		//if (glm::dot(direction, normalWS) > 0) {
		//	return Renderer::Instance()->GetEmissive(modelIndex, meshIndex);
		//}
        
        return Renderer::Instance()->models[modelIndex]->meshes[meshIndex].AreaLight->L(direction, it);
        
	}

	return { 0,0,0 };

}
