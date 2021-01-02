

#include "base/header.h"
#include "base/renderer.h"
#include "vertex.h"
Vertex::Vertex(dVec3 _v, dVec3 _n, dVec2 _uv, dMat4 _objectToWorld, int _modelIndex) {

	modelIndex = _modelIndex;
	objectToWorld = _objectToWorld;
	v = _v;
	uv = _uv;
	normalOS = glm::normalize(_n);
	dVec4 nor = objectToWorld * dVec4(normalOS, 0);
	normalWS = glm::normalize(dVec3{ nor.x, nor.y, nor.z });
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
//	//����ʹ�õ���uniform scale������ֱ����ת���������uniform scale����Ҫ������ת
//	//��ת���� glm::transpose(glm::inverse(objectToWorld));
//	//normalWS = objectToWorld * dVec4(normalOS,1);
//
//	normalWS = glm::normalize(glm::transpose(glm::inverse(objectToWorld)) * dVec4(normalOS, 1));
//}


