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
	
}; 

class Model {
public:

	Model();
	~Model();

	void Initialize(const char* path);
	void deInitialize();

	int GetIndexCount();
	void GetIndices(std::vector<int>&);

	int GetMeshCount();

	void ProcessNode(aiNode *node, const aiScene *scene);

	std::vector<MeshInfo> meshes;

	int GetVertexCount();
	void GetVertices(std::vector<glm::vec3>&vertices);
	void GetNormals(std::vector<glm::vec3>&normals);
	void GetUVs(std::vector<glm::vec2>&uvs);
	void GetMeshIndices(std::vector<int>&);

	Assimp::Importer importer;
	const aiScene *aiObj;


	bool HasNormal();

	bool hasNormal;
};