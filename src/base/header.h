
#pragma once

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <math.h>

#define FLOAT double
#define  SIZE  500
#define CHANNEL_COUNT  3
#define AOV 60
#define DEPTH 5
#define dVec2 glm::dvec2
#define dVec3 glm::dvec3
#define dVec4 glm::dvec4
#define dMat4 glm::dmat4
#define dMat3 glm::dmat3

#define Mat4 glm::mat4
#define Mat3 glm::mat3
#define Vec3 glm::vec3
#define Vec4 glm::vec4

#define RAY_LENGTH 1000

#define OUTPUT_PATH "../assets/image/result.jpg"

#define EULER_TO_RADIUS 0.0174532925199438  // pi/180

inline FLOAT eulerToRadius(float euler) {
	return (EULER_TO_RADIUS * euler);
}


class Model;
class Geometrys;
class Ray;
class RayTracer;
class Renderer;
class Shape;
struct IntersectPoint;
class BoundingBox;
class Sphere;
class Triangle;
class BusinessCard;
class WhiteColor;
class Vertex;
class Accelerate;
class Linear;
class  Bvh;

