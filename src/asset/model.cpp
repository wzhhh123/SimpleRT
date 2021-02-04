#include "base/header.h"
#include "model.h"
#include "bxdf/lambert.h"


Model::Model()
{
	hasNormal = true;
}


void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		MeshInfo meshInfo;
		meshInfo.mesh = mesh;

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

		meshInfo.bxdf = new Lambert(meshInfo.diffuse);

		meshes.push_back(meshInfo);

		//
		//第七个是区域光
		if (meshes.size() == 8) {
			meshes[meshes.size() - 1].isAreaLight = true;
			meshes[meshes.size() - 1].emissive = { 1 ,1, 1, 1};
		}
		else {
			meshes[meshes.size()-1].isAreaLight = false;
		}

		//后面六个没有uv

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

	// 接下来对它的子节点重复这一过程
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
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
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


void Model::GetIndices(std::vector<int>& indices) {

	indices.resize(GetIndexCount());
	int index = 0;
	int indexOffset = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (auto j = 0; j < meshes[i].mesh->mNumFaces; ++j) {
			indices[index++] = meshes[i].mesh->mFaces[j].mIndices[0] + indexOffset;
			indices[index++] = meshes[i].mesh->mFaces[j].mIndices[1] + indexOffset;
			indices[index++] = meshes[i].mesh->mFaces[j].mIndices[2] + indexOffset;
		}
		indexOffset += meshes[i].mesh->mNumVertices;
	}

}

void Model::GetMeshIndices(std::vector<int>& meshIndices) {

	//简单粗暴地返回网格id
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


void Model::GetUVs(std::vector<glm::vec2>&uvs) {
	int len = GetVertexCount();
	uvs.resize(len);
	int index = 0;

	for (auto i = 0; i < meshes.size(); ++i) {
		//meshes[i].mesh->mNumUVComponents[0]全是2  不知道为什么 换成mNumVertices
		for (auto j = 0; j < meshes[i].mesh->mNumVertices; ++j) {
			uvs[index++] = { meshes[i].mesh->mTextureCoords[0][j].x, meshes[i].mesh->mTextureCoords[0][j].y };
			//	std::cout << uvs[index-1].x << " " << uvs[index - 1].x << std::endl;
		}
	}
}



void Model::GetTangents(std::vector<glm::vec3>&tangents) {
	tangents.resize(GetVertexCount());
	int index = 0;
	for (auto i = 0; i < meshes.size(); ++i) {

		if (meshes[i].mesh->HasTangentsAndBitangents()) {
			for (int j = 0; j < meshes[i].mesh->mNumVertices; ++j) {
				tangents[index++] = {
				meshes[i].mesh->mTangents[j].x,
				meshes[i].mesh->mTangents[j].y,
				meshes[i].mesh->mTangents[j].z
				};
			}
		}
		else {
			std::cout << "fuckyou" << std::endl;
		}
	}
}



void Model::GetBitangents(std::vector<glm::vec3>&bittangents) {
	bittangents.resize(GetVertexCount());
	int index = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (int j = 0; j < meshes[i].mesh->mNumVertices; ++j) {
			bittangents[index++] = {
			meshes[i].mesh->mBitangents[j].x,
			meshes[i].mesh->mBitangents[j].y,
			meshes[i].mesh->mBitangents[j].z
			};
		}
	}
}



int Model::GetVertexCount()
{
	int verticeCnt = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		verticeCnt += meshes[i].mesh->mNumVertices;
	}
	return verticeCnt;
}


void Model::GetNormals(std::vector<glm::vec3>&normals) {
	normals.resize(GetVertexCount());
	if (!hasNormal) {
		std::cout << "mesh has no normal!" << std::endl;
		return;
	}

	int index = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (auto j = 0; j < meshes[i].mesh->mNumVertices; ++j) {
			normals[index++] = { meshes[i].mesh->mNormals[j].x , meshes[i].mesh->mNormals[j].y, meshes[i].mesh->mNormals[j].z };
		}
	}
}

bool Model::HasNormal() {
	return hasNormal;
}

void Model::GetVertices(std::vector<glm::vec3>&vertices)
{
	vertices.resize(GetVertexCount());

	float div = 1;
	int index = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (auto j = 0; j < meshes[i].mesh->mNumVertices; ++j) {
			vertices[index++] = { meshes[i].mesh->mVertices[j].x / div, meshes[i].mesh->mVertices[j].y / div, meshes[i].mesh->mVertices[j].z / div };
		}
	}

}