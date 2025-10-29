#include "../include/color.h"
#include "../include/vector.h"
#include "../include/ppm.h"


int openPPMFile(FILE** f, const char* fname){
    *f = fopen(fname, "w");

    if(*f != NULL)
        return 1;

    printf("Error opening file: %s\n", strerror(errno));
    return -1;
}


void initPPMHeader(FILE* f, int n, int m){
    char* str = (char*)malloc(50);
    sprintf(str, "P6 \n%d %d\n%d ", n, m, RGBMAX);
    
    fwrite(str, strlen(str) * sizeof(char), strlen(str), f);
    free(str);
}

void writePPMPixels(FILE* f, RGB* pixels, const int size){
    fwrite(pixels, sizeof(RGB), size, f);
}

RGB** initBuffer(int xSize, int ySize){
    RGB** buffer = malloc(ySize * sizeof(RGB *));
    buffer[0] = (RGB*)malloc(xSize * ySize * sizeof(RGB));
    
    for(int i=1; i < ySize; i++)
        buffer[i] = buffer[0] + (i * xSize);
    
    return buffer;
}


void writeBufferToPPM(RGB **buffer, FILE* f, int xSize, int ySize){
    for(int i=0; i < ySize; i++)
        writePPMPixels(f, buffer[i], xSize);
}


RGB applyBrightness(RGB color, double brightness){
    color.r = fmin(color.r * brightness, RGBMAX);
    color.g = fmin(color.g * brightness, RGBMAX);
    color.b = fmin(color.b * brightness, RGBMAX);

    return color;
}


// vec3 functionailty dit product adding etc etc
vec3 add(vec3 a, vec3 b){
    vec3 product;

    product.x = a.x + b.x;
    product.y = a.y + b.y;
    product.z = a.z + b.z;

    return product;
}

vec3 scalerMultiply(vec3 a, double n){
    vec3 product;

    product.x = a.x * n;
    product.y = a.y * n;
    product.z = a.z * n;

    return product;
}

vec3 subtract(vec3 a, vec3 b){
    b = scalerMultiply(b, -1);
    return add(a, b);
}

vec3 divideScaler(vec3 a, double n){
    return scalerMultiply(a, 1/n);
}

double magnitude(vec3 a){
    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

double dotProduct(vec3 a, vec3 b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

double calcAngle(vec3 a, vec3 b){
    return acos(dotProduct(a, b) / (magnitude(a)*magnitude(b)));
}

vec3 calcUnitVector(vec3 a){
    return divideScaler(a, magnitude(a));
}

vec3 crossProduct(vec3 a, vec3 b){
    vec3 result; 

    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);

    return result;
}


vec3 calcRayPos(ray r, double t){
    return add(scalerMultiply(r.direction, t), r.origin);
}

// calculates a . (b * c) used for ray triangle intersection
// The triple produict of three vectors is also the determant of [A B C]
// As they both are equal to the volume of the parrelogram
double tripleProduct(vec3 a, vec3 b, vec3 c){
    return dotProduct(a, crossProduct(b, c));
}

int rayTriangleIntersection(const Triangle triangle, ray r, double *t){
    vec3 edge1 = subtract(triangle.v2, triangle.v1);
    vec3 edge2 = subtract(triangle.v3, triangle.v1);

    vec3 planeNormal = crossProduct(edge1, edge2);

    if(dotProduct(planeNormal, r.direction) == 0) // ray is perpindiculor to the plane 
        return -1;

    vec3 nD = scalerMultiply(r.direction, -1);
    vec3 O = subtract(r.origin, triangle.v1);
    
    double det = tripleProduct(nD, edge1, edge2);

    // check lambda1 and lambda2 for negtive values
    // 0 < l1,l2,l3 < 1 when the point intersects    
    double det1 = tripleProduct(nD, O, edge2);
    double lambda1 = det1 / det;

    if(lambda1 < 0 || lambda1 > 1) 
        return -1;

    det1 = tripleProduct(nD, edge1, O); 
    double lambda2 = det1 / det;

    if(lambda2 < 0 || (lambda1 + lambda2) > 1)
        return -1;

    det1 = tripleProduct(O, edge1, edge2); // use cramers rule to solve for t
    *t = det1 / det; 

    return 0;
}


bool isIntersectingCircle(const Circle circle, const ray r){
    vec3 OC = subtract(r.origin, circle.centre);

    double a = pow(magnitude(r.direction), 2);
    double b = -2 * (dotProduct(r.direction, OC));
    double c = dotProduct(OC, OC) - pow(circle.radius, 2);

    return ((b*b) - (4 * a * c));  
}

bool isIntersectingCircleCasted(const void* circleVoid, const ray r){
    Circle *circle = (Circle*)circleVoid;

    return isIntersectingCircle(*circle, r);
}


bool isIntersectingTriangleCasted(const void* triangleVoid, const ray r){
    Triangle* triangle = (Triangle*)triangleVoid;
    double t = 0;
    
    return rayTriangleIntersection(*triangle, r,&t) == 0;
}

RGB calcRayColor(ray r, struct hittableObject world[WORLD_SIZE]){
    RGB red = {255, 0, 0};
    RGB blue = {0, 0, 245};

    for (int i=0; i < WORLD_SIZE; i++){
        if (world[i].isHit(world[i].object, r)) {
            return red;
        }
    }
    
    return blue;
}


void printVec(vec3 a){
    printf(" %f %f %f", a.x, a.y, a.z);
}

int main(){
    double aspectRatio = 16.0 / 9.0;
    
    int imageWidth = XSIZE;
    int imageHeight = (int)(imageWidth / aspectRatio);

    double viewportWidth = 2.0;
    double viewportHeight = viewportWidth / aspectRatio;

    double deltaU = viewportWidth / imageWidth;
    double deltaV = viewportHeight / imageHeight;

    int focalPoint = 1;

    vec3 cameraOrigin = {0, 0, 0};
    
    struct hittableObject world[WORLD_SIZE];
    
    Triangle triangle = {
        {0, 0, focalPoint}, 
        {0.5, 0.5, focalPoint},
         {1, 0, focalPoint}
    };

    struct hittableObject triangleHittable = {&triangle, &isIntersectingTriangleCasted};
    world[0] = triangleHittable;
    
    RGB** buffer = initBuffer(imageWidth, imageHeight);
    for(int i=0; i < imageHeight; i++){
        for(int j=0; j < imageWidth; j++){
            vec3 pixelCentre = {j * deltaU, i * deltaV, focalPoint};
            vec3 rayDirection = calcUnitVector(pixelCentre);

            ray r = {cameraOrigin, rayDirection};
            RGB color = calcRayColor(r, world);
        
            buffer[i][j] = color;
        }
    }

    FILE* f = NULL; 
    int result = openPPMFile(&f, "test.ppm");

    if (result == -1)
        return -1;
    
    initPPMHeader(f, imageWidth, imageHeight);
    writeBufferToPPM(buffer, f, imageWidth, imageHeight);

    free(buffer[0]);
    free(buffer);
    
    fclose(f);
}