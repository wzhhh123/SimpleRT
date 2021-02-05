#include "math.h"


dMat4 getViewMatrixRTL(dVec3 cameraPos, dVec3 target, dVec3 up) {

	dVec3 front = glm::normalize(target - cameraPos);
	dVec3 right = glm::normalize(glm::cross(front, up));
	dVec3 viewUp = glm::normalize(glm::cross(right, front));

	dMat4 worldToView;
	worldToView[0] = { right.x, right.y, right.z, 0 };
	worldToView[1] = { viewUp.x, viewUp.y, viewUp.z, 0 };
	worldToView[2] = { front.x, front.y, front.z, 0 };
	worldToView[3] = { -dot(right, cameraPos), -dot(viewUp, cameraPos), -dot(front, cameraPos) ,1 };

	return worldToView;
}



void coordinateSystem(const dVec3 &normal, dVec3 &tangent, dVec3 &bitangent)
{
	if (std::abs(normal.x) > std::abs(normal.y)) {
		float invLen = 1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		bitangent = dVec3(normal.z * invLen, 0.0f, -normal.x * invLen);
	}
	else {
		float invLen = 1.0f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
		bitangent = dVec3(0.0f, normal.z * invLen, -normal.y * invLen);
	}
	tangent = glm::normalize(glm::cross(bitangent, normal));
	bitangent = glm::normalize(glm::cross(normal, tangent));
}
