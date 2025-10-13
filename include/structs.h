#ifndef STRUCT
#define STRUCT
typedef char uint8_t;

typedef struct{
    double x, y, z;
}vec3;

typedef struct{
    vec3 origin;
    vec3 direction; 
}ray;

typedef struct {
    uint8_t r, b, g;
}RGB;
#endif