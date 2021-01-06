
#include "triangle.h"
#include "base/renderer.h"


//https://www.cnblogs.com/samen168/p/5162337.html
Triangle::Triangle(dVec3 _v0, dVec3 _v1, dVec3 _v2, dVec3 _n0, dVec3 _n1, dVec3 _n2, dVec2 _uv0, dVec2 _uv1, dVec2 _uv2, dMat4 model, int _modelIndex,int _meshIndex) :
	objectToWorld(model), modelIndex(_modelIndex), meshIndex(_meshIndex)
{
	if (Renderer::Instance()->models[modelIndex]->hasNormal) {
		v0 = { _v0, _n0, _uv0, model, modelIndex };
		v1 = { _v1, _n1, _uv1, model, modelIndex };
		v2 = { _v2, _n2, _uv2, model, modelIndex };
	}
	else {
		//看了一下nori 手动计算一下面法线
		dVec3 e1 = _v1 - _v0;
		dVec3 e2 = _v2 - _v0;
		dVec3 nor = glm::normalize(glm::cross(e1, e2));
		v0 = { _v0, nor, _uv0, model, modelIndex };
		v1 = { _v1, nor, _uv1, model, modelIndex };
		v2 = { _v2, nor, _uv2, model, modelIndex };
	}
	boundingBox = BoundingBox(v1.vertexWS, v2.vertexWS);
	boundingBox.Union(v0.vertexWS);
}

//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
//Möller–Trumbore intersection algorithm
//克莱姆法则+向量混合积
bool Triangle::Intersect(Ray r, IntersectPoint& p)
{

	const float EPSILON = 0.0000001;
	dVec3 vertex0 = v0.vertexWS;
	dVec3 vertex1 = v1.vertexWS;
	dVec3 vertex2 = v2.vertexWS;
	dVec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(r.direction, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	f = 1.0 / a;
	s = r.origin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(r.direction, q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		p.t = t;
		p.weightU = u;
		p.weightV = v;

		p.normalWS = v1.normalWS *p.weightU + v2.normalWS * p.weightV + v0.normalWS * (1 - p.weightU - p.weightV);
		//p.normalOS = v1.normalOS * p.weightU + v2.normalOS * p.weightV + v0.normalOS * (1 - p.weightU - p.weightV);
		p.normalOS = v1.normalOS;
		p.meshIndex = meshIndex;
		p.modelIndex = modelIndex;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}




//用到法线贴图再上这个
//void Triangle::TangentToObject() {
//	float deltaU1 = _uv1.x - _uv0.x;
//	float deltaU2 = _uv2.x - _uv0.x;
//
//	float deltaV1 = _uv1.y - _uv0.y;
//	float deltaV2 = _uv2.y - _uv0.y;
//
//	dVec3 e1 = _v1 - _v0;
//	dVec3 e2 = _v2 - _v0;
//
//	//列优先，不影响计算
//	auto mid = glm::mat2x2(deltaV2, -deltaU2, -deltaV1, deltaU1);
//	auto right = glm::mat3x2(e1.x, e2.x, e1.y, e2.y, e1.z, e2.z);
//	float left = 1.0 / (deltaU1*deltaV2 - deltaU2 * deltaV1);
//	auto res = left * mid * right;
//
//	tangent = dVec3{ res[0][0] , res[1][0] , res[2][0] };
//	bitangent = dVec3{ res[0][1] , res[1][1] , res[2][1] };
//	auto zComp = glm::cross(tangent, bitangent);
//
//	tangentToObject = dMat3{ tangent.x, bitangent.x, zComp.x, tangent.y, bitangent.y, zComp.y, tangent.z, bitangent.z, zComp.z };
//}
