#pragma once

#include "geometry/triangle.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
//AABB BoundingBox



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

	std::vector<aiMesh*> meshes;

	int GetVertexCount();
	void GetVertices(std::vector<glm::vec3>&vertices);
	void GetNormals(std::vector<glm::vec3>&normals);
	void GetUVs(std::vector<glm::vec2>&uvs);


	Assimp::Importer importer;
	const aiScene *aiObj;


	bool HasNormal();

	bool hasNormal;
};