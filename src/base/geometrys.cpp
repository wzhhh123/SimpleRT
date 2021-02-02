

#include "geometrys.h"
#include "asset/model.h"
#include "sampling.h"

Geometrys* Geometrys::Instance() {

	static Geometrys *geometrys = new Geometrys();
	return geometrys;

}


bool Geometrys::Intersect(Ray r, IntersectPoint* p){
	r.origin += 1e-6*r.direction;
	return accelerater->Intersect(r, p);
}

Geometrys::Geometrys() : isInit(false) {  }

void Geometrys::Initialize(std::vector<Model*>& models, std::vector<dMat4>& objectToWorlds) {

	assert(isInit == false);
	isInit = !isInit;

	int triangleCnt = 0;
	for (auto i = 0; i < models.size(); ++i) {
		triangleCnt += models[i]->GetIndexCount();
	}
	triangleCnt /= 3;

	shapes.clear();
	shapes.resize(triangleCnt);

	int cnt = 0;
	for (auto j = 0; j < models.size(); ++j) {

		std::vector < glm::vec3 > vertices;
		models[j]->GetVertices(vertices);
		std::vector < glm::vec3 > normals;
		models[j]->GetNormals(normals);
		std::vector <int> indices;
		models[j]->GetIndices(indices);
		std::vector < glm::vec2 > uvs;
		models[j]->GetUVs(uvs);
		std::vector <int> meshIndex;
		models[j]->GetMeshIndices(meshIndex);

		//std::vector<glm::vec3>tangents;
		//models[j]->GetTangents(tangents);

		//std::vector<glm::vec3>bitangents;
		//models[j]->GetBitangents(bitangents);

		for (auto i = 0; i < indices.size(); i += 3) {

			//printf("(%02f,%02f,%02f) (%02f,%02f,%02f) (%02f,%02f,%02f)", 
			//	tangents[i].x,
			//	tangents[i].y, 
			//	tangents[i].z, 
			//	tangents[i].x, 
			//	tangents[i].y, 
			//	tangents[i].z, 
			//	tangents[i].x, 
			//	tangents[i].y, 
			//	tangents[i].z );

			shapes[cnt] = new Triangle(

				dVec3(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z),
				dVec3(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z),
				dVec3(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z),

				dVec3(normals[indices[i]].x, normals[indices[i]].y, normals[indices[i]].z),
				dVec3(normals[indices[i + 1]].x, normals[indices[i + 1]].y, normals[indices[i + 1]].z),
				dVec3(normals[indices[i + 2]].x, normals[indices[i + 2]].y, normals[indices[i + 2]].z),

				dVec2(uvs[indices[i]].x, uvs[indices[i]].y),
				dVec2(uvs[indices[i + 1]].x, uvs[indices[i + 1]].y),
				dVec2(uvs[indices[i + 2]].x, uvs[indices[i + 2]].y),

				objectToWorlds[j],
				j,
				meshIndex[i]
			); 


			if (meshIndex[i] == 7) {
				lights.push_back(shapes[cnt]);
				lightShapeIndices.push_back(cnt);
			}

			++cnt;

		}
	}


	std::vector<FLOAT> lightDis;
	for (int i = 0; i < lights.size(); ++i) {
		lightDis.push_back(dynamic_cast<Triangle*>(shapes[i])->Area());
	}

	lightDistribute = Distribution1D(lightDis.data(), lightDis.size());

	accelerater->Initialize(&shapes);
}

