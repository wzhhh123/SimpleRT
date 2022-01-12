
#pragma once

#include <fstream>

#include <string>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <math.h>


#define FLOAT double
#define  IMG_SIZE  480
#define SPP 1000
#define CHANNEL_COUNT  3
#define AOV 60
#define DEPTH 8
#define THREAD_COUNT 64
#define dVec2 glm::dvec2
#define dVec3 glm::dvec3
#define dVec4 glm::dvec4
#define dMat4 glm::dmat4
#define dMat3 glm::dmat3

#define Mat4 glm::mat4
#define Mat3 glm::mat3
#define Mat2 glm::mat2
#define Vec3 glm::vec3
#define Vec4 glm::vec4

const FLOAT Pi = 3.14159265358979323846;
const FLOAT InvPi = 0.31830988618379067154;
const FLOAT Inv2Pi = 0.15915494309189533577;
const FLOAT Inv4Pi = 0.07957747154594766788;
const FLOAT PiOver2 = 1.57079632679489661923;
const FLOAT PiOver4 = 0.78539816339744830961;
const FLOAT Sqrt2 = 1.41421356237309504880;

#define RAY_LENGTH 1000

#if __APPLE__
#define OUTPUT_PATH "../../assets/image/result_apple.jpg"
#define CONFIG_PATH "../../assets/configs/configtest_apple.json"
#else
#define OUTPUT_PATH "../assets/image/result.jpg"
#define CONFIG_PATH "../assets/configs/configtest.json"
#endif


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
class IntersectPoint;
class BoundingBox;
class Sphere;
class Triangle;
class BusinessCard;
class WhiteColor;
class Vertex;
class Accelerate;
class Linear;
class  Bvh;
class BxDF;
class Lambert;
class Path;
class Distribution1D;
class Timer;


struct TileRenderThreadInput;
struct TileRenderThreadOutput;
