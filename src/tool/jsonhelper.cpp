#include "jsonhelper.h"



void AddString(rapidjson::Document::AllocatorType& allocator, rapidjson::Value& father, rapidjson::GenericStringRef<char>name, rapidjson::GenericStringRef<char>val) {
	father.AddMember(name, val, allocator);
}


void AddVec3(rapidjson::Document::AllocatorType& allocator, rapidjson::Value& father, rapidjson::GenericStringRef<char>name, glm::vec3 val) {
	using namespace std;
	using namespace rapidjson;
	Value arr(kArrayType);
	arr.PushBack(val.x, allocator);
	arr.PushBack(val.y, allocator);
	arr.PushBack(val.z, allocator);
	father.AddMember(name, arr, allocator);
}

