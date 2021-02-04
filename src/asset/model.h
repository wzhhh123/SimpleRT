#pragma once

#include "geometry/triangle.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
//AABB BoundingBox



struct MeshInfo {

	aiMesh* mesh;
	glm::vec4 diffuse;
	glm::vec4 ambient;
	glm::vec4 specular;
	glm::vec4 emissive;
	glm::vec4 shininess;

	BxDF* bxdf;

	bool isAreaLight;
	
}; 

class Model {
public:

	Model();
	~Model();

	void Initialize(const char* path);
	void deInitialize();

	int GetIndexCount();


	int GetMeshCount();

	void ProcessNode(aiNode *node, const aiScene *scene);

	std::vector<MeshInfo> meshes;

	int GetVertexCount();

	void GetMeshIndices(std::vector<int>&);

	int Model::GetFaceCount(int meshIndex);
	aiFace* GetFaces(int meshIndex);
	aiVector3D * Model::GetVertices(int meshIndex);
	aiVector3D * Model::GetNormals(int meshIndex);
	aiVector3D * Model::GetUVs(int meshIndex);
	aiVector3D * Model::GetTangents(int meshIndex);
	aiVector3D *  Model::GetBitangents(int meshIndex);


	Assimp::Importer importer;
	const aiScene *aiObj;


	bool HasNormal();

	bool hasNormal;
};