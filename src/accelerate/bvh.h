#pragma once

#include "base/header.h"
#include "geometry/triangle.h"
#include "base/accelerate.h"
#include "geometry/boundingbox.h"


struct TreeNode {
public:
	std::vector<int>indices;
	BoundingBox curBox;
	std::vector<TreeNode*>sons;
	bool isLeaf;
};

class Bvh : public Accelerate {

public:

	virtual void Initialize(std::vector<Shape*>*scene);
	virtual bool Intersect(Ray& r, IntersectPoint* p, int* index);



	std::vector<Shape*>* scene;

private:


	TreeNode* treeNode;

	TreeNode* BuildTree(BoundingBox box, std::vector<int>&, int depth);

	bool SearchTree(Ray& r, IntersectPoint* p, int * index, TreeNode* treeNode);

	int maxDepth = 0;
};
