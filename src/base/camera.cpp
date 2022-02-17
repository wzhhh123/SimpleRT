

#include "camera.h"
#include "ray.h"

Camera::Camera()
{
	dirZ = IMG_SIZE / (tan(AOV * acos(-1) / 360) * 2); // 360/PI~=114  计算近平面离相机距离 //照顾一下aabb计算 使用左手系

	dVec3 o = glm::normalize(dVec3{ 0,0,dirZ });
	dVec3 ox = glm::normalize(dVec3{ 1,0,dirZ });
	dVec3 oy = glm::normalize(dVec3{ 0,1,dirZ });

	dxCamera = ox - o;
	dyCamera = oy - o;

}

void Camera::GenerateRay(int Idx, dVec2 Sample,  Ray& R)
{
	dVec3 dir;
	dir.x = Idx % IMG_SIZE - IMG_SIZE / 2 + Sample.x;
	dir.y = IMG_SIZE / 2 - Idx / IMG_SIZE + Sample.y;
	dir.z = dirZ;
	R.origin = dVec3{ 0,0,0 };
	R.direction = glm::normalize(dir);
}


