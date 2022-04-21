

#include "geometrys.h"
#include "asset/model.h"
#include "sampling.h"

Geometrys* Geometrys::Instance() {

	static Geometrys *geometrys = new Geometrys();
	return geometrys;

}


bool Geometrys::Intersect(Ray r, IntersectPoint* p) {
	//r.origin += 1e-6*r.direction;
	return accelerater->Intersect(r, p);
}

Geometrys::Geometrys() : isInit(false) {  }

void Geometrys::Initialize(std::vector<Model*>& models, std::vector<dMat4>& objectToWorlds) {

	assert(isInit == false);
	isInit = !isInit;

	int faceCnt = 0;
	for (auto i = 0; i < models.size(); ++i) {
		for(int j = 0; j < models[i]->meshes.size(); ++j)
		faceCnt += models[i]->GetFaceCount(j);
	}

	shapes.clear();
	shapes.resize(faceCnt);

	int cnt = 0;
	for (auto j = 0; j < models.size(); ++j) {

		for (int m = 0; m < models[j]->meshes.size(); ++m) {
			
			aiFace* faces = models[j]->GetFaces(m);
			aiVector3D* vertices = models[j]->GetVertices(m);
			int faceNum = models[j]->GetFaceCount(m);
			for (auto i = 0; i < faceNum; ++i) {
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
					dVec3(vertices[faces[i].mIndices[0]].x, vertices[faces[i].mIndices[0]].y, vertices[faces[i].mIndices[0]].z),
					dVec3(vertices[faces[i].mIndices[1]].x, vertices[faces[i].mIndices[1]].y, vertices[faces[i].mIndices[1]].z),
					dVec3(vertices[faces[i].mIndices[2]].x, vertices[faces[i].mIndices[2]].y, vertices[faces[i].mIndices[2]].z),
					objectToWorlds[j],
					j,
					m,
					i
				);

				if (models[j]->meshes[m].isAreaLight) {
					lights.push_back(shapes[cnt]);
					lightShapeIndices.push_back(cnt);
				}

				++cnt;
			}
		}
	}


	//pbrt3�õ�light������������
	std::vector<FLOAT> lightDis(lights.size(), 1);
	//for (int i = 0; i < lights.size(); ++i) {
	//	lightDis.push_back(dynamic_cast<Triangle*>(shapes[i])->Area());
	//}
	lightDistribute = Distribution1D(lightDis.data(), lightDis.size());

	accelerater->Initialize(&shapes);
}

