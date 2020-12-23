#include "base/header.h"
#include "model.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model() {

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile("../assets/models/Common_Bench_A.FBX", aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

}
