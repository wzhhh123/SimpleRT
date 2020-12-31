
#include "triangle.h"





Triangle::Triangle(dVec3 _v0, dVec3 _v1, dVec3 _v2, dVec3 _n0, dVec3 _n1, dVec3 _n2, dVec2 _uv0, dVec2 _uv1, dVec2 _uv2, dMat4 model): 
	v0(_v0, _n0, _uv0, model), v1(_v1, _n1, _uv1, model), v2(_v2, _n2, _uv2, model), objectToWorld(model)
{


	
	//v0.SetTangentToObject(tangentToObject);
	//v1.SetTangentToObject(tangentToObject);
	//v2.SetTangentToObject(tangentToObject);


}

//克莱姆法则+向量混合积
bool Triangle::Intersect(Ray r, IntersectPoint& p)
{

	auto orig = r.origin;
	auto dir = r.direction;

	// E1
	dVec3 E1 = v1.vertexWS - v0.vertexWS;

	// E2
	dVec3 E2 = v2.vertexWS - v0.vertexWS;

	// P
	dVec3 P = glm::cross(dir, E2);

	// determinant
	FLOAT det = glm::dot(E1, P);

	dVec3 T;
	if (det > 0)
	{
		T = orig - v0.vertexWS;
	}
	else
	{
		T = v0.vertexWS  - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	// Calculate u and make sure u <= 1
	p.weightU = glm::dot(T, P);
	if (p.weightU < 0.0 || p.weightU > det)
		return false;

	// Q
	dVec3 Q = glm::cross(T, E1);

	// Calculate v and make sure u + v <= 1
	p.weightV = glm::dot(dir, Q);
	if (p.weightV < 0.0 || p.weightU + p.weightV > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	p.t = glm::dot(E2, Q);

	FLOAT fInvDet = 1.0 / det;
	p.t *= fInvDet;
	p.weightU *= fInvDet;
	p.weightV *= fInvDet;

	p.normalWS = v1.normalWS;// *p.weightU + v2.normalWS * p.weightV + v0.normalWS * (1 - p.weightU - p.weightV);
	p.normalOS = v1.normalOS * p.weightU + v2.normalOS * p.weightV + v0.normalOS * (1 - p.weightU - p.weightV);

	
	return true;

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
