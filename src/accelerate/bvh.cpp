

#include "bvh.h"


void Bvh::Initialize(std::vector<Shape*>*scene)
{
	this->scene = scene;
	std::vector<int> indices;
	auto size = scene->size();
	indices.resize(size);
	for (auto i = 0; i < size; ++i) indices[i] = i;
	BoundingBox box = dynamic_cast<Triangle*>((*scene)[indices[0]])->boundingBox;
	for (int i = 0; i < indices.size(); ++i) {
		box.Union(dynamic_cast<Triangle*>((*scene)[i])->boundingBox);
	}
	treeNode = BuildTree(box, indices, 0);
	std::cout << "maxDepth: " << maxDepth << std::endl;
}


//index to shape array in bvh
TreeNode* Bvh::BuildTree(BoundingBox box, std::vector<int>&indices, int depth) {
	

	maxDepth = std::max(depth, maxDepth);
	
	TreeNode* curNode = new TreeNode();
	curNode->isLeaf = true;
	curNode->indices.reserve(indices.size());
	for (int i = 0; i < indices.size(); ++i) {
		if (box.Cross(dynamic_cast<Triangle*>((*scene)[indices[i]])->boundingBox)) {
			curNode->indices.push_back(indices[i]);
		}
	}
	curNode->curBox = box;
	
	if (curNode->indices.size() == 0) {
		delete curNode;
		return nullptr;
	}
	/*
		std::cout << "("
			<< curNode->curBox.amin.x << ","
			<< curNode->curBox.amin.y << ","
			<< curNode->curBox.amin.z << ")"
			<< " "
			<< "("
			<< curNode->curBox.amax.x << ","
			<< curNode->curBox.amax.y << ","
			<< curNode->curBox.amax.z << ")"
			<< " "
			<< curNode->indices.size()
			<< std::endl;
	*/
	if (curNode->indices.size() <= 15) return curNode;

	int maxExtent = box.MaxExtent();
	if (box.amax[maxExtent] - box.amin[maxExtent] < 0.05) {
#if DEBUG_MODE
		//看看是否会出现在一个小的aabb内还有大量三角形
		std::cout << "Bvh stop partite because max extent  of aabb less than 0.05, leaf triangle count is " << indices.size() << std::endl;
#endif
		return curNode;//最大维度才0.2，再划分下去可能没啥意义
	}



	dVec3 midPoint = (box.amax + box.amin)*0.5;
	
	std::vector<BoundingBox> sonBoxes(8);

	sonBoxes[0] = { box.amin, midPoint };
	sonBoxes[1] = { {midPoint.x, box.amin.y, box.amin.z},  {box.amax.x, midPoint.y, midPoint.z} };
	sonBoxes[2] = { {box.amin.x, midPoint.y, box.amin.z},  {midPoint.x, box.amax.y, midPoint.z} };
	sonBoxes[3] = { {midPoint.x, midPoint.y, box.amin.z},  {box.amax.x, box.amax.y, midPoint.z}};

	dVec3 extent = { 0,0,(box.amax.z - box.amin.z) * 0.5 };

	sonBoxes[4] = { dVec3{box.amin.x, box.amin.y, midPoint.z}, dVec3{midPoint.x, midPoint.y, box.amax.z} };
	sonBoxes[5] = { dVec3{midPoint.x, box.amin.y, midPoint.z}, dVec3{box.amax.x, midPoint.y, box.amax.z} };
	sonBoxes[6] = { dVec3{box.amin.x, midPoint.y, midPoint.z}, dVec3{midPoint.x, box.amax.y, box.amax.z} };
	sonBoxes[7] = { dVec3{midPoint.x, midPoint.y, midPoint.z}, dVec3{box.amax.x, box.amax.y, box.amax.z} };

	curNode->sons.resize(8);

	for (int i = 0; i < 8; ++i) {
		curNode->sons[i] = BuildTree(sonBoxes[i], curNode->indices, depth + 1);
	}
	curNode->isLeaf = false;
	return curNode;
}

bool Bvh::Intersect(Ray& r, IntersectPoint* p)
{
	return SearchTree(r, p, treeNode);
}

bool Bvh::SearchTree(Ray& r, IntersectPoint* p, TreeNode* treeNode) {

	bool found = false;
	IntersectPoint nearestHit = IntersectPoint();
	Shape* boundingBox = &(treeNode->curBox);

	//这里不知道为什么不能直接调用boundingbox的instersect，要转到shape调用
	if (boundingBox->Intersect(r, nearestHit)) {
		if (treeNode->isLeaf) {
			for (size_t i = 0; i < treeNode->indices.size(); ++i)
			{
				IntersectPoint p;
				Shape* shape = (*scene)[treeNode->indices[i]];
				if (shape->Intersect(r, p)) {
					p.index = treeNode->indices[i];
					if (!found) {
						found = true;
						nearestHit = p;
					}
					else if (p.t < nearestHit.t) {
						nearestHit = p;
					}
				}
			}
		}
		else {
			for (int i = 0; i < 8; ++i) {
				IntersectPoint p;
				if (treeNode->sons[i] != nullptr && SearchTree(r, &p, treeNode->sons[i])) {
					if (!found) {
						found = true;
						nearestHit = p;
					}
					else if(p.t < nearestHit.t){
						nearestHit = p;
					}
				}
			}
		}
	}

	if (found) *p = nearestHit;
	
	return found;
}
