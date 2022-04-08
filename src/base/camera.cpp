

#include "camera.h"
#include "ray.h"

Camera::Camera()
{
	dirZ = IMG_SIZE / (tan(FOV * acos(-1) / 360) * 2); // 360/PI~=114  计算近平面离相机距离 //照顾一下aabb计算 使用左手系

	dVec3 o = glm::normalize(dVec3{ 0,0,dirZ });
	dVec3 ox = glm::normalize(dVec3{ 1,0,dirZ });
	dVec3 oy = glm::normalize(dVec3{ 0,1,dirZ });

	dxCamera = ox - o;
	dyCamera = oy - o;

}

void Camera::GenerateRay(int Idx, dVec2 Sample, Ray& R)
{
	dVec3 dir;
	dir.x = Idx % IMG_SIZE - IMG_SIZE / 2 + Sample.x + 0.5;
	dir.y = IMG_SIZE / 2 - Idx / IMG_SIZE + Sample.y + 0.5;
	dir.z = dirZ;
	R.origin = dVec3{ 0,0,0 };
	R.direction = glm::normalize(dir);
}

void Camera::GenerateRay(Point2i pFilm, dVec2 Sample, Ray& R)
{
    dVec3 dir;
    dir.x = float(pFilm.x - IMG_SIZE / 2) + Sample.x;
    dir.y = float(IMG_SIZE / 2 - pFilm.y) + Sample.y;
    dir.z = dirZ;
    R.origin = dVec3{ 0,0,0 };
    R.direction = glm::normalize(dir);
}


void Camera::GenerateRayDifferential(int Idx, dVec2& Sample, RayDifferential *Rd)
{
    GenerateRay(Idx, Sample, *Rd);
    {
        Ray rx;
        dVec2 SampleX = Sample;
        SampleX.x += 1;
        GenerateRay(Idx, SampleX, rx);
        Rd->rxOrigin = Rd->origin + (rx.origin - Rd->origin)/1.0;
        Rd->rxDirection = Rd->direction + (rx.direction - Rd->direction)/1.0;
    }
    {
        Ray ry;
        dVec2 SampleY = Sample;
        SampleY.y += 1;
        GenerateRay(Idx, SampleY, ry);
        Rd->ryOrigin = Rd->origin + (ry.origin - Rd->origin)/1.0;
        Rd->ryDirection = Rd->direction + (ry.direction - Rd->direction)/1.0;
    }
    Rd->bHasDifferentials = true;
    return;
}
