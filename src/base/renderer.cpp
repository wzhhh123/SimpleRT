#include "header.h"
#include "renderer.h"
#include "datatoimage.h"
#include "glm.hpp"
#include "ray.h"
#include "geometrys.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "geometry/triangle.h"
#include "gtc/matrix_transform.hpp"

void Renderer::Run()
{

	int yx = 0;
	while (yx < SIZE * SIZE) {

		dVec3 dir;
		dir.x = yx % SIZE - SIZE / 2;
		dir.y = SIZE / 2 - yx / SIZE;
		dir.z = SIZE  / (tan(AOV * acos(-1) / 360) * 2); // 360/PI~=114  计算近平面离相机距离

		Ray r = {};
		r.origin = dVec3{ 0,0,0 };
		r.direction = glm::normalize(dir);
	
		imageData[yx] = raytracer->Trace(DEPTH, r) * 255.0;
		++yx;
	}

	DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = new dVec3[SIZE*SIZE];

	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ -0.57, -1, 2.3698 }, VEC3{ 1., 1., 1. }, 0.5, .05, .2, .85, 0., 1.7));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 0.63, -1, 2.369 }, VEC3{ 1., .5, .2 }, 0.5, .7, .3, 0., 0.05, 1.2));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 0.01, 0.43, 3.15 }, VEC3{ .1, .8, .8 }, 1., .3, .7, 0., 0., 1.2));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 3.67, 8.99, 5.64 }, VEC3{ 1., .8, 1. }, 3.5, 0., 0., 0., .6, 1.5));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ -5.02, 6.67, 5.97 }, VEC3{ .8, 1., 1. }, 2.5, 0., 0., 0., .5, 1.5));


	//Geometrys::Instance()->shapes.push_back(
	//	new Triangle(VEC3{ 0.5, -0.5,2}, VEC3{ 0.5, 0.5,2 }, VEC3{ -0.5, 0.5,2 }));
	//Geometrys::Instance()->shapes.push_back(
	//	new Triangle(VEC3{ -0.5, 0.5,2 }, VEC3{ -0.5, -0.5,2 }, VEC3{ 0.5, -0.5,2 }));


	dMat4 scale = glm::scale(dMat4(1.0f), dVec3{ 90,90,90 });
	dMat4 trans = glm::translate(scale, dVec3{ 0, -0.5, 2 });



	const aiScene* scene = model.obj;
	for (unsigned int i = 0; i < model.obj->mNumMeshes; ++i) {
		aiVector3D* vertices = (scene)->mMeshes[i]->mTextureCoords[0];
		for (unsigned int p = 0; p < model.obj->mMeshes[i]->mNumFaces; ++p)
		{
			aiFace face = (scene->mMeshes[i]->mFaces[p]);
			dVec3 posX = trans * dVec4(vertices[face.mIndices[0]].x, vertices[face.mIndices[0]].y, vertices[face.mIndices[0]].z, 1);
			dVec3 posY = trans * dVec4(vertices[face.mIndices[1]].x, vertices[face.mIndices[1]].y, vertices[face.mIndices[1]].z, 1);
			dVec3 posZ = trans * dVec4(vertices[face.mIndices[2]].x, vertices[face.mIndices[2]].y, vertices[face.mIndices[2]].z, 1);
			Geometrys::Instance()->shapes.push_back(new Triangle(posX, posY, posZ));

			std::cout << vertices[face.mIndices[0]].x << " " << vertices[face.mIndices[0]].y << " " << vertices[face.mIndices[0]].z << std::endl;
			std::cout << vertices[face.mIndices[1]].x << " " << vertices[face.mIndices[1]].y << " " << vertices[face.mIndices[1]].z << std::endl;
			std::cout << vertices[face.mIndices[2]].x << " " << vertices[face.mIndices[2]].y << " " << vertices[face.mIndices[2]].z << std::endl;
		}
	}



}

void Renderer::deIntialize() {

	delete[] imageData;
}

Renderer::Renderer() {}
