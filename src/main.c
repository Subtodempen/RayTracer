#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <math.h>

#define XSIZE 8
#define RGBMAX 255


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

static inline int magnitude(vec3 a);
static inline int dotProduct(vec3 a, vec3 b);

// read and write to the ppm file and functionbailty
//int readPPM(FILE* f);
void writePPMPixels(FILE* f, RGB* pixels, const int size);
void initPPMHeader(FILE* f, int n, int m);
int openPPMFile(FILE** f, const char* fname);

RGB** initBuffer(int xSize, int ySize);
void writeBufferToPPM(RGB **buffer, FILE* f, int xSize, int ySize);

int openPPMFile(FILE** f, const char* fname){
    *f = fopen(fname, "w");

    if(*f != NULL)
        return 1;

    printf("Error opening file: %s\n", strerror(errno));
    return -1;
}


void initPPMHeader(FILE* f, int n, int m){
    char* str = (char*)malloc(50);
    sprintf(str, "P6 \n%d %d\n255 ", n , m);
    
    fwrite(str, strlen(str) * sizeof(char), strlen(str), f);
    free(str);
}

void writePPMPixels(FILE* f, RGB* pixels, const int size){
    fwrite(pixels, size / sizeof(RGB), size, f);
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


// vec3 functionailty dit product adding etc etc
vec3 add(vec3 a, vec3 b){
    vec3 product;

    product.x = a.x + b.x;
    product.y = a.y + b.y;
    product.z = a.z + b.z;

    return product;
}

vec3 scalerMultiply(vec3 a, int n){
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

vec3 divide(vec3 a, int n){
    return scalerMultiply(a, 1/n);
}

int magnitude(vec3 a){
    return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

int dotProduct(vec3 a, vec3 b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vec3 calcUnitVector(vec3 a){
    return divide(a, magnitude(a));
}

vec3 calcRayPos(ray r, int t){
    return add(scalerMultiply(r.direction, t), r.origin);
}

RGB calcRayColor(ray r){
    RGB a = {0, 0, 0};
    return a;
}

int main(){
    double aspectRatio = 16.0 / 9.0;
    
    int imageWidth = XSIZE;
    int imageHeight = imageWidth / (int)aspectRatio;

    double viewportWidth = 2.0;
    double viewportHeight = viewportWidth / aspectRatio;

    double deltaU = imageWidth / viewportWidth;
    double deltaV = imageHeight / viewportHeight;

    int focalPoint = 1;

    vec3 cameraOrigin = {0, 0, 0};

    RGB** buffer = initBuffer(imageWidth, imageHeight);
    for(int i=0; i < imageWidth; i++){
        for(int j=0; j < imageHeight; j++){
            vec3 pixelCentre = {i * deltaU, j * deltaV, focalPoint};
            vec3 rayDirection = calcUnitVector(pixelCentre);

            ray r = {cameraOrigin, rayDirection};
            RGB color = calcRayColor(r);
        
            buffer[i][j] = color;
        }
    }

    FILE* f = NULL; 
    int result = openPPMFile(&f, "test.ppm");

    if (result == -1)
        return -1;
    
    initPPMHeader(f, imageWidth, imageHeight);
    writeBufferToPPM(buffer, f, imageWidth, imageHeight);

    free(buffer);
    fclose(f);
}