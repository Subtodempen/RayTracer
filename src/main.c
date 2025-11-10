#include "../include/color.h"
#include "../include/vector.h"
#include "../include/ppm.h"
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


int openPPMFile(FILE** f, const char* fname){
    *f = fopen(fname, "w");

    if(*f != NULL)
        return 1;

    printf("Error opening file: %s\n", strerror(errno));
    return -1;
}


void initPPMHeader(FILE* f, int n, int m){
    char* str = (char*)malloc(50);
    sprintf(str, "P6\n%d %d\n%d\n", n, m, RGBMAX);
    
    fwrite(str, sizeof(char), strlen(str), f);
    free(str);
}

void writePPMPixels(FILE* f, RGB* pixels, const int size){
    fwrite(pixels, sizeof(RGB), size, f);
}

RGB** initBuffer(int xSize, int ySize){
    RGB** buffer = malloc(ySize * sizeof(RGB *));
    buffer[0] = (RGB*)malloc(xSize * ySize * sizeof(RGB));
    
    for(int i=0; i < ySize; i++)
        buffer[i] = buffer[0] + (i * xSize);
    
    return buffer;
}


void writeBufferToPPM(RGB **buffer, FILE* f, int xSize, int ySize){
    for(int i = ySize - 1; i >= 0; i--)
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
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;

    return a;
}

vec3 scalerMultiply(vec3 a, double n){
    a.x = a.x * n;
    a.y = a.y * n;
    a.z = a.z * n;

    return a;
}

vec3 subtract(vec3 a, vec3 b){
    b = scalerMultiply(b, -1);
    return add(a, b);
}

vec3 divideScaler(vec3 a, double n){
    return scalerMultiply(a, 1.0/n);
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

int rayTriangleIntersection(const Triangle triangle, ray r, hitRecord *h){
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
    
    h->t = det1 / det; 
    h->hPoint = calcRayPos(r, h->t);
    h->normal = crossProduct(edge1, edge2);

    return 0;
}


int isIntersectingCircle(const Circle circle, const ray r, hitRecord* h){
    vec3 OC = subtract(r.origin, circle.centre);

    double a = pow(magnitude(r.direction), 2);
    double b = 2 * (dotProduct(r.direction, OC));
    double c = dotProduct(OC, OC) - pow(circle.radius, 2);

    double disc = (b*b) - (4*a*c);

    if(disc < 0 || c < 0)
        return -1;
        
    double t1 = (-b - sqrt(disc)) / (2 * a);
    double t2 = (-b + sqrt(disc)) / (2 * a);

    if(fabs(t1) < fabs(t2))
        h->t = t1;
    
    else
        h->t = t2;
        
    h->hPoint = calcRayPos(r, h->t);
    h->normal = calcUnitVector(subtract(h->hPoint, circle.centre));
    
    return 0;
        
}

bool isIntersectingCircleCasted(const void* circleVoid, const ray r, hitRecord* h){
    Circle *circle = (Circle*)circleVoid;

    return isIntersectingCircle(*circle, r, h) == 0;
}


bool isIntersectingTriangleCasted(const void* triangleVoid, const ray r, hitRecord* h){
    Triangle* triangle = (Triangle*)triangleVoid;
    
    return rayTriangleIntersection(*triangle, r, h) == 0;
}

int trace(ray r, hitRecord* h, const struct hittableObject world[], const unsigned int worldSize){
    double minDistance = DBL_MAX;
    hitRecord *minRecord = NULL;
    
    for(int i=0; i < worldSize; i++){
        if(world[i].isHit(world[i].object, r, h)){
            //double currDistance = h->t;

            //if(minDistance > currDistance){
               // minDistance = currDistance;
                //minRecord = h;
            //}
            return 0;
        }
    }

    if(minRecord == NULL)
        return -1;
    
    return 0;
}   


vec3 randomVec(){
    vec3 randVec; 

    randVec.x = rand();
    randVec.y = rand();
    randVec.z = rand();

    return calcUnitVector(randVec);
}


vec3 randomVecAtNormal(vec3 normal){
   vec3 randVec = randomVec();
    
    if (dotProduct(normal, randVec) > 0)
        return randVec;
    else
        return scalerMultiply(randVec, -1);

}
    
RGB vecToRGB(vec3 a){
    RGB result;

    result.r = a.x;
    result.g = a.y;
    result.b = a.z;

    return result;
}
RGB calcRayColor(ray r, const struct hittableObject world[], const unsigned int worldSize, int depth){
    RGB black = {0, 0, 0};

    if(depth <= 0)
        return black;
    
    hitRecord h;

    if(trace(r, &h, world, worldSize) == 0){
        vec3 lightDir = randomVecAtNormal(h.normal);
        ray lightRay = {h.hPoint, lightDir};
        
        return applyBrightness(calcRayColor(lightRay, world, worldSize, depth - 1), 0.75);
    }
    
    double a = 0.5 * (r.direction.y + 1);

    vec3 startColor = {0, 0, 0};
    vec3 endColor = {100, 200, 255};

    return vecToRGB(add(scalerMultiply(startColor, 1 - a), scalerMultiply(endColor, a)));
}

void render(RGB** buffer, 
    struct hittableObject world[],
    vec3 cameraOrigin, 
    int imageWidth, 
    int imageHeight,
    double viewportWidth, 
    double viewportHeight,
    int focalPoint){

    vec3 viewportMiddle = {0.5 * viewportWidth, 0.5 * viewportHeight, 0};
    vec3 viewportBottomLeft = subtract(cameraOrigin, viewportMiddle);

    double deltaU = viewportWidth / imageWidth;
    double deltaV = viewportHeight / imageHeight;

    vec3 halfDistance = {deltaU * 0.5, deltaV * 0.5, 0};
    vec3 pixel0 = add(viewportBottomLeft, halfDistance);
    
    for(int i=0; i < imageHeight; i++){
        for(int j=0; j < imageWidth; j++){
            vec3 pixelCentre = {j * deltaU,  i * deltaV, focalPoint};
            pixelCentre = add(pixel0, pixelCentre);
            
            vec3 rayDirection = calcUnitVector(pixelCentre);
            ray r = {cameraOrigin, rayDirection};

            RGB color = calcRayColor(r, world, WORLD_SIZE, MAX_RECURSION_DEPTH);
            buffer[i][j] = color;
        }
    }
}


void printVec(vec3 a){
    printf(" %f %f %f", a.x, a.y, a.z);
}

int main(){
    double aspectRatio = 16.0 / 9.0;
    
    int imageWidth = XSIZE;
    int imageHeight = (int)(imageWidth / aspectRatio);
    
    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * aspectRatio;
    
    int focalPoint = 1;
    struct hittableObject world[WORLD_SIZE];
    vec3 cameraOrigin = {0, 0,0};
    
    Circle c = {{0, 0,focalPoint}, 0.5};
    struct hittableObject hCircle = {&c, &isIntersectingCircleCasted};
    
    Circle groundCircle = {{0, -100.5, focalPoint}, 100};
    struct hittableObject groundHCircle = {&groundCircle, &isIntersectingCircleCasted}; 

    world[0] = hCircle;
    world[1] = groundHCircle;
    
    RGB** buffer = initBuffer(imageWidth, imageHeight);
    
    render(
        buffer, 
        world, 
        cameraOrigin, 
        imageWidth, 
        imageHeight, 
        viewportWidth, 
        viewportHeight, 
        focalPoint);

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