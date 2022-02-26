#pragma once

#include "geometry/triangle.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "lights/diffuse.h"
#include "base/material.h"
//AABB BoundingBox



struct MeshInfo {

	aiMesh* mesh;
	glm::vec4 diffuse;
	glm::vec4 ambient;
	glm::vec4 specular;
	glm::vec4 emissive;
	glm::vec4 shininess;
    
	Texture<dVec3>* albedoTex;

    Material* material;

	bool isAreaLight;
    
    std::shared_ptr<AreaLight> AreaLight;
    
    MeshInfo()
    {
        //bsdf = nullptr;
        material = nullptr;
        isAreaLight = false;
    }
    
    ~MeshInfo()
    {
        //if(bsdf) delete bsdf;
        if(material) delete material;
    }
    
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

    int GetFaceCount(int meshIndex);
	aiFace* GetFaces(int meshIndex);
    aiVector3D * GetVertices(int meshIndex);
    aiVector3D * GetNormals(int meshIndex);
    aiVector3D * GetUVs(int meshIndex);
    aiVector3D * GetTangents(int meshIndex);
    aiVector3D *  GetBitangents(int meshIndex);


	Assimp::Importer importer;
	const aiScene *aiObj;


	bool HasNormal();

	bool hasNormal;
};
