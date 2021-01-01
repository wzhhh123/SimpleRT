
#include "boundingbox.h"




BoundingBox::BoundingBox()
{

}


void BoundingBox::Union(BoundingBox& a) {
	amax = glm::max(a.amax, amax);
	amin = glm::min(a.amin, amin);
}


void BoundingBox::Union(dVec3& p) {

	amax = glm::max(p, amax);
	amin = glm::min(p, amin);

}

void BoundingBox::Intersect(BoundingBox& a) {

	amin = glm::max(amin, a.amin);
	amax = glm::min(amax, a.amax);

}


bool BoundingBox::Intersect(Ray r, IntersectPoint& p)
{
	static const float EPS = 1e-6f;
	// 光线方向
	FLOAT d[3];
	d[0] = r.direction.x * RAY_LENGTH;
	d[1] = r.direction.y * RAY_LENGTH;
	d[2] = r.direction.z * RAY_LENGTH;
	// 因为是线段 所以参数t取值在0和1之间
	FLOAT tmin = 0.0;
	FLOAT tmax = 1.0;

	for (int i = 0; i < 3; i++)
	{
		// 如果光线某一个轴分量为 0，且在包围盒这个轴分量之外，那么直接判定不相交 
		if (fabsf(d[i]) < EPS)
		{
			if (r.origin[i] < amin[i] || r.origin[i] > amax[i])
				return false;
		}
		else
		{
			const float ood = 1.0f / d[i];
			// 计算参数t 并令 t1为较小值 t2为较大值
			float t1 = (amin[i] - r.origin[i]) * ood;
			float t2 = (amax[i] - r.origin[i]) * ood;
			if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }

			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			// 判定不相交
			if (tmin > tmax) return false;
		}
	}

	return true;

}



BoundingBox Union(const BoundingBox& a, const BoundingBox& b) {

	BoundingBox box;
	box.amax = glm::max(a.amax, b.amax);
	box.amin = glm::min(a.amin, b.amin);
	return box;

}

BoundingBox Union(const BoundingBox& a, const dVec3& b) {

	BoundingBox box;
	box.amax = glm::max(a.amax, b);
	box.amin = glm::min(a.amin, b);
	return box;

}

BoundingBox Intersect(const BoundingBox& a, const BoundingBox& b) {

	BoundingBox box;
	box.amin = glm::max(b.amin, a.amin);
	box.amax = glm::min(b.amax, a.amax);
	return box;

}
