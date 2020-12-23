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

	Assimp::Importer importer;
	const aiScene *obj;
	
};