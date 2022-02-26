#include "base/header.h"
#include "model.h"
#include "bxdf/lambert.h"
#include "lights/diffuse.h"

Model::Model()
{
	hasNormal = true;
}


void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		MeshInfo meshInfo;
		meshInfo.mesh = mesh;

		if (0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			aiColor4D color;
			if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS) {
				meshInfo.diffuse = { color.r, color.g, color.b, color.a };
			}
			if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS) {
				meshInfo.ambient = { color.r, color.g, color.b, color.a };
			}
			if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS) {
				meshInfo.emissive = { color.r, color.g, color.b, color.a };
			}
			if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS) {
				meshInfo.specular = { color.r, color.g, color.b, color.a };
			}
			if (aiGetMaterialColor(mat, AI_MATKEY_SHININESS, &color) == AI_SUCCESS) {
				meshInfo.shininess = { color.r, color.g, color.b, color.a };
			}
            
            meshInfo.material = CreateMatteMaterial(meshInfo.diffuse, nullptr);
            //meshInfo.bsdf->Add(std::make_shared<LambertianRefrection>(meshInfo.diffuse));
            //meshInfo.bsdf->Add(new LambertianRefrection(meshInfo.diffuse));
		}
		meshes.push_back(meshInfo);

	//	hasNormal &= mesh->HasNormals();

		std::cout << std::endl;
		/*		 mat.name
				$mat.shadingm
				$clr.ambient
				$clr.diffuse
				$clr.specular
				$clr.emissive
				$mat.shininess
				$mat.opacity
				$clr.transparent
				$mat.refracti*/

	}

	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Model::~Model()
{
}

void Model::Initialize(const char* path)
{
	//const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |// aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	aiObj = scene;

	for (auto i = 0; i < aiObj->mNumTextures; ++i) {
		std::cout << aiObj->mTextures[i]->mFilename.C_Str() << std::endl;
	}

	ProcessNode(aiObj->mRootNode, aiObj);
    
    //for(int i = 0; i < meshes.size(); ++i)
    //{
    //    int faceNum = meshes[i].mesh->mNumFaces;
    //    for(int j = 0; j < faceNum; ++j)
    //    {
    //        aiFace face = meshes[i].mesh->mFaces[j];
    //        std::cout << "indexnum: " << face.mNumIndices << std::endl;
    //        std::cout
    //        << face.mIndices[0] << " "
    //        << face.mIndices[1] << " "
    //        << face.mIndices[2] << " "
    //        << std::endl;
    //
    //        aiVector3D* uv = meshes[i].mesh->mTextureCoords[0];
    //        std::cout
    //        << uv[face.mIndices[0]].x << " "<< uv[face.mIndices[0]].y << " "<< uv[face.mIndices[0]].z << " "
    //        << uv[face.mIndices[1]].x << " "<< uv[face.mIndices[1]].y << " "<< uv[face.mIndices[1]].z << " "
    //        << uv[face.mIndices[2]].x << " "<< uv[face.mIndices[2]].y << " "<< uv[face.mIndices[2]].z << " "
    //        << std::endl;
    //    }
    //}
    
}

void Model::deInitialize()
{
	importer.FreeScene();
}


int Model::GetIndexCount() {

	int indexCnt = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		indexCnt += meshes[i].mesh->mNumFaces * 3;
#if DEBUG_MODE
		for (auto j = 0; j < meshes[i].mesh->mNumFaces; ++j) {
			assert(meshes[i].mesh->mFaces[j].mNumIndices == 3);
		}
#endif
	}
	return indexCnt;
}

int Model::GetMeshCount() {
	return (int)meshes.size();
}

aiFace *Model::GetFaces(int meshIndex) {
	return meshes[meshIndex].mesh->mFaces;
}

int Model::GetFaceCount(int meshIndex) {
	return  meshes[meshIndex].mesh->mNumFaces;
}

void Model::GetMeshIndices(std::vector<int>& meshIndices) {

	//�򵥴ֱ��ط�������id
	meshIndices.resize(GetIndexCount());
	int index = 0;
	int indexOffset = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (auto j = 0; j < meshes[i].mesh->mNumFaces; ++j) {
			meshIndices[index++] = i;
			meshIndices[index++] = i;
			meshIndices[index++] = i;
		}
	}
}

aiVector3D * Model::GetUVs(int meshIndex) {
	return meshes[meshIndex].mesh->mTextureCoords[0];
}

aiVector3D * Model::GetTangents(int meshIndex) {
	return meshes[meshIndex].mesh->mTangents;
}

aiVector3D *  Model::GetBitangents(int meshIndex) {
	return  meshes[meshIndex].mesh->mBitangents;
}


int Model::GetVertexCount()
{
	int verticeCnt = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		verticeCnt += meshes[i].mesh->mNumVertices;
	}
	return verticeCnt;
}

aiVector3D * Model::GetNormals(int meshIndex) {
	return  meshes[meshIndex].mesh->mNormals;
}

bool Model::HasNormal() {
	return hasNormal;
}

aiVector3D * Model::GetVertices(int meshIndex) {
	return  meshes[meshIndex].mesh->mVertices;
}

