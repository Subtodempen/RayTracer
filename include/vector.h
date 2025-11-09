#ifndef RAY
#define RAY

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>


#include "color.h"

#define WORLD_SIZE 2
#define MAX_RECURSION_DEPTH 10

typedef struct{
    double x, y, z;
}vec3;

typedef struct{
    vec3 origin;
    vec3 direction; 
}ray;

typedef struct{
    vec3 v1, v2, v3; 
}Triangle;

typedef struct{
    vec3 centre; 
    double radius;
}Circle;

typedef struct{
    double t; 
    vec3 hPoint;
    vec3 normal;
}hitRecord;

// abstractable object every function should type cast object into specified
// turn into a linked list later
struct hittableObject{
    void* object;
    bool (*isHit)(const void* object, ray r,  hitRecord* h);
};


vec3 calcRayPos(ray r, double t);
RGB calcRayColor(ray r, const struct hittableObject world[], const unsigned int worldSize, int depth);
int trace(ray r, hitRecord* h, const struct hittableObject world[], const unsigned int worldSize);

int isIntersectingCircle(const Circle circle, const ray r, hitRecord *h);
bool isIntersectingTriangleCasted(const void* triangleVoid, const ray r, hitRecord *h);

int rayTriangleIntersection(const Triangle triangle, ray r, hitRecord *h);

// vector functionailty
static inline vec3 add(vec3, vec3);
static inline vec3 scalerMultiply(vec3, double);
static inline vec3 divideScaler(vec3, double);
static inline vec3 subtract(vec3, vec3);

static inline double magnitude(vec3);
static inline double calcAngle(vec3, vec3);

static inline double dotProduct(vec3, vec3);
static inline vec3 crossProduct(vec3, vec3);
static inline double tripleProduct(vec3, vec3, vec3);

static inline vec3 randomVec();
static inline vec3 randomVecAtNormal(vec3 normal);
#endif