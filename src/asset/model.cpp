#include "base/header.h"
#include "model.h"


Model::Model() {

	const aiScene *scene = importer.ReadFile("../assets/models/box.fbx", aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	obj = scene;

}


Model::~Model() {
	importer.FreeScene();
}
