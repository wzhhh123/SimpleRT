

#include "base/header.h"
#include "base/renderer.h"
#include "vertex.h"


Vertex::Vertex(dVec3 _v, dMat4 _objectToWorld, int _modelIndex) {

	modelIndex = _modelIndex;
	objectToWorld = _objectToWorld;
	v = _v;
	vertexWS = objectToWorld * dVec4{ v,1 };

}

Vertex::Vertex() {

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


