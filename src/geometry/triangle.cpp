
#include "triangle.h"
#include "base/renderer.h"
#include "tool/math.h"



FLOAT Triangle::Area() {

	auto e1 = v2.vertexWS - v0.vertexWS;
	auto e2 = v1.vertexWS - v0.vertexWS;
	return glm::length(glm::cross(e1, e2)) / 2;

}


dVec2 UniformSampleTriangle(const dVec2 &u) {
	FLOAT su0 = std::sqrt(u[0]);
	return dVec2(1 - su0, u[1] * su0);
}

void Triangle::SetData(IntersectPoint& it) {

	aiVector3D* normal = Renderer::Instance()->models[it.modelIndex]->GetNormals(meshIndex);
	if (normal) {

		aiFace* face = Renderer::Instance()->models[it.modelIndex]->GetFaces(meshIndex);

		int idx0 = face[it.faceIndex].mIndices[0];
		int idx1 = face[it.faceIndex].mIndices[1];
		int idx2 = face[it.faceIndex].mIndices[2];

		dVec3 worldNormal0 = objectToWorld * dVec4{ normal[idx0].x, normal[idx0].y, normal[idx0].z, 0 };
		dVec3 worldNormal1 = objectToWorld * dVec4{ normal[idx1].x, normal[idx1].y, normal[idx1].z, 0 };
		dVec3 worldNormal2 = objectToWorld * dVec4{ normal[idx2].x, normal[idx2].y, normal[idx2].z, 0 };

		it.normalWS =
			glm::normalize(
				it.weightU * worldNormal1 +
				it.weightV * worldNormal2 +
				(1 - it.weightV - it.weightU) * worldNormal0);

		coordinateSystem(it.normalWS, it.tangentWS, it.bitangentWS);

		it.tangentToWorld = { it.tangentWS, it.bitangentWS, it.normalWS };
		it.worldToTangent = glm::inverse(it.tangentToWorld);
	}
	else {
		std::cout << "no normals???" << std::endl;
	}

	//it.uv = it.weightU * v0.uv + it.weightV  * v1.uv + (1 - it.weightV - it.weightU) * v2.uv;

}

IntersectPoint Triangle::Samping(dVec2 point, FLOAT* pdf) {

	IntersectPoint it;
	dVec2 b = UniformSampleTriangle(point);
	it.weightU = b.x;
	it.weightV = b.y;

	it.modelIndex = modelIndex;
	it.meshIndex = meshIndex;
	it.faceIndex = faceIndex;
	SetData(it);
	*pdf = 1.0 / Area();

	return it;
}


//https://www.cnblogs.com/samen168/p/5162337.html
Triangle::Triangle(dVec3 _v0, dVec3 _v1, dVec3 _v2, dMat4 model, int _modelIndex,int _meshIndex, int _faceIndex) :
	objectToWorld(model), modelIndex(_modelIndex), meshIndex(_meshIndex), faceIndex(_faceIndex)
{
	//先这样hack一下  后面几个平面都是没uv的，但是又有uv的值 全是(0,1)，简直智障
	//if (_meshIndex >= 2) {
	//	_uv0 = dVec2(0, 0);
	//	_uv1 = dVec2(1, 1);
	//	_uv2 = dVec2(1, 0);
	//}
	//if (Renderer::Instance()->models[modelIndex]->hasNormal) {

		v0 = { _v0,  model, modelIndex };
		v1 = { _v1, model, modelIndex };
		v2 = { _v2, model, modelIndex };
	//}
	//else {
	//	//看了一下nori 手动计算一下面法线
	//	dVec3 e1 = _v1 - _v0;
	//	dVec3 e2 = _v2 - _v0;
	//	dVec3 nor = glm::normalize(glm::cross(e1, e2));
	//	v0 = { _v0, nor, _uv0, _t0, _bt0, model, modelIndex };
	//	v1 = { _v1, nor, _uv1, _t1, _bt1,model, modelIndex };
	//	v2 = { _v2, nor, _uv2, _t2, _bt2,model, modelIndex };
	//}
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

		dVec2 uv[3];
		GetUVs(uv);
		dVec2 duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
		dVec3 dp02 = v0.vertexWS - v2.vertexWS, dp12 = v1.vertexWS - v2.vertexWS;
		FLOAT determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
		bool degenerateUV = std::abs(determinant) < 1e-8;
		dVec3 dpdu, dpdv;
		//if (!degenerateUV) 
		{
			FLOAT invdet = 1 / determinant;
			dpdu = (duv12[1] * dp02 - duv02[1] * dp12) * invdet;
			dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invdet;
		}

		//p.normalWS = glm::normalize(glm::cross(dpdv, dpdu));
		//dVec3 tangent = glm::normalize(dpdu);
		//dVec3 bitangent = glm::normalize(glm::cross(tangent, p.normalWS));

		////use face normal
		//if (1) {
		//	p.normalWS = v1.normalWS *p.weightU + v2.normalWS * p.weightV + v0.normalWS * (1 - p.weightU - p.weightV);
		//	tangent = v1.tangentWS *p.weightU + v2.tangentWS * p.weightV + v0.tangentWS * (1 - p.weightU - p.weightV);
		//	bitangent = v1.bitangentWS *p.weightU + v2.bitangentWS * p.weightV + v0.bitangentWS * (1 - p.weightU - p.weightV);
		//}

		p.meshIndex = meshIndex;
		p.modelIndex = modelIndex;
		p.faceIndex = faceIndex;

		SetData(p);

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


void Triangle::GetUVs(dVec2 uv[3]) {

	uv[0] = dVec2(0, 0);
	uv[1] = dVec2(1, 1);
	uv[2] = dVec2(1, 0);
	/*	uv[0] = v0.uv;
		uv[1] = v1.uv;
		uv[2] = v2.uv;*/
}
