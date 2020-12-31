

#include "base/header.h"

#include "vertex.h"
Vertex::Vertex(dVec3 _v, dVec3 _n, dVec2 _uv, dMat4 _objectToWorld) {

	objectToWorld = _objectToWorld;
	v = _v;
	uv = _uv;
	normalOS = glm::normalize(_n);

	dMat4 rotation = glm::rotate(glm::dmat4(1.0), eulerToRadius(0), dVec3{ 0,1,0 });
	rotation = glm::rotate(rotation, eulerToRadius(180), dVec3{ 1,0,0 });
	rotation = glm::rotate(rotation, eulerToRadius(0), dVec3{ 0,0,1 });
	dMat4 scale = glm::scale(rotation, dVec3{ .01,.01,.01 });
	//normalWS = glm::normalize(glm::transpose(glm::inverse(objectToWorld)) * dVec4(normalOS, 0));
	normalWS = glm::normalize(scale * dVec4(normalOS, 0));
	vertexWS = objectToWorld * dVec4{ v,1 };

}

//
//void Vertex::SetTangentToObject(dMat3 _tangentToObject) {
//
//	tangentToObject = _tangentToObject;
//
//	normalOS = glm::normalize(_tangentToObject * n);
//
//	//假设使用的是uniform scale，可以直接逆转，如果不是uniform scale，需要手算逆转
//	//逆转矩阵 glm::transpose(glm::inverse(objectToWorld));
//	//normalWS = objectToWorld * dVec4(normalOS,1);
//
//	normalWS = glm::normalize(glm::transpose(glm::inverse(objectToWorld)) * dVec4(normalOS, 1));
//}


