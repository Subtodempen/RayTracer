#ifndef RAY
#define RAY

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "color.h"

#define WORLD_SIZE 1

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

// abstractable object every function should type cast object into specified
// turn into a linked list later
struct hittableObject{
    void* object;
    bool (*isHit)(const void* object, ray r);
};

vec3 calcRayPos(ray r, double t);
RGB calcRayColor(ray r, struct hittableObject world[WORLD_SIZE]);

bool isIntersectingCircle(const Circle circle, const ray r);
bool isIntersectingTriangleCasted(const void* triangleVoid, const ray r);

int rayTriangleIntersection(const Triangle triangle, ray r, double *t);

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

#endif