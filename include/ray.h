#define XSIZE 8
#define RGBMAX 255

typedef char uint8_t;

typedef struct {
    uint8_t r, b, g;
}RGB;

typedef struct{
    double x, y, z;
}vec3;


typedef struct{
    vec3 origin;
    vec3 direction; 
}ray;

vec3 calcRayPos(ray r, int t);
RGB calcRayColor(ray r);

// vector functionailty
static inline vec3 add(vec3 a, vec3 b);
static inline vec3 scalerMultiply(vec3 a, int n);
static inline vec3 subtract(vec3 a, vec3 b);

inline int magnitude(vec3 a);
static inline int dotProduct(vec3 a, vec3 b);
