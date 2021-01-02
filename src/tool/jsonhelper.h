#pragma once

#include "base/header.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


void AddString(rapidjson::Document::AllocatorType& allocator, rapidjson::Value& father, rapidjson::GenericStringRef<char>name, rapidjson::GenericStringRef<char>val);
void AddVec3(rapidjson::Document::AllocatorType& allocator, rapidjson::Value& father, rapidjson::GenericStringRef<char>name, glm::vec3 val);