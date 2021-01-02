#include "base/header.h"
#include "model.h"


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
		meshes.push_back(mesh);
		hasNormal &= mesh->HasNormals();
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
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
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
		indexCnt += meshes[i]->mNumFaces * 3;
#if DEBUG_MODE
		for (auto j = 0; j < meshes[i]->mNumFaces; ++j) {
			assert(meshes[i]->mFaces[j].mNumIndices == 3);
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
		for (auto j = 0; j < meshes[i]->mNumFaces; ++j) {
			indices[index++] = meshes[i]->mFaces[j].mIndices[0] + indexOffset;
			indices[index++] = meshes[i]->mFaces[j].mIndices[1] + indexOffset;
			indices[index++] = meshes[i]->mFaces[j].mIndices[2] + indexOffset;
		}
		indexOffset += meshes[i]->mNumVertices;
	}

}



void Model::GetUVs(std::vector<glm::vec2>&uvs) {
	uvs.resize(GetVertexCount());
	int index = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		for (auto j = 0; j < meshes[i]->mNumUVComponents[0]; ++j) {
			uvs[index++] = { meshes[i]->mTextureCoords[0][j].x, meshes[i]->mTextureCoords[0][j].y };
		}
	}
}


int Model::GetVertexCount()
{
	int verticeCnt = 0;
	for (auto i = 0; i < meshes.size(); ++i) {
		verticeCnt += meshes[i]->mNumVertices;
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
		for (auto j = 0; j < meshes[i]->mNumVertices; ++j) {
			normals[index++] = { meshes[i]->mNormals[j].x , meshes[i]->mNormals[j].y, meshes[i]->mNormals[j].z };
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
		for (auto j = 0; j < meshes[i]->mNumVertices; ++j) {
			vertices[index++] = { meshes[i]->mVertices[j].x / div, meshes[i]->mVertices[j].y / div, meshes[i]->mVertices[j].z / div };
		}
	}

}