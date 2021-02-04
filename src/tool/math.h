#pragma once

#include "base/header.h"
#include <chrono>



dMat4 getViewMatrixRTL(dVec3 cameraPos, dVec3 target, dVec3 up);

void coordinateSystem(const dVec3 &normal, dVec3 &tangent, dVec3 &bitangent);




