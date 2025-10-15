#ifndef RAY
#define RAY

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "color.h"

typedef struct{
    double x, y, z;
}vec3;

typedef struct{
    vec3 origin;
    vec3 direction; 
}ray;

vec3 calcRayPos(ray r, double t);
RGB calcRayColor(ray r);
bool isIntersectingCircle(vec3 origin, double radius, const ray r);

// vector functionailty
static inline vec3 add(vec3 a, vec3 b);
static inline vec3 scalerMultiply(vec3 a, double n);
static inline vec3 divideScaler(vec3 a, double n);
static inline vec3 subtract(vec3 a, vec3 b);

static inline double magnitude(vec3 a);
static inline double dotProduct(vec3 a, vec3 b);
#endif