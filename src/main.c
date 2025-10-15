#include "../include/color.h"
#include "../include/ray.h"
#include "../include/ppm.h"
#include <math.h>
#include <stdio.h>


int openPPMFile(FILE** f, const char* fname){
    *f = fopen(fname, "w");

    if(*f != NULL)
        return 1;

    printf("Error opening file: %s\n", strerror(errno));
    return -1;
}


void initPPMHeader(FILE* f, int n, int m){
    char* str = (char*)malloc(50);
    sprintf(str, "P6 \n%d %d\n255 ", n, m);
    
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

vec3 calcUnitVector(vec3 a){
    return divideScaler(a, magnitude(a));
}

vec3 calcRayPos(ray r, double t){
    return add(scalerMultiply(r.direction, t), r.origin);
}


bool isIntersectingCircle(vec3 centre, double radius, const ray r){
    vec3 OC = subtract(r.origin, centre);

    double a = pow(magnitude(r.direction), 2);
    double b = -2 * (dotProduct(r.direction, OC));
    double c = dotProduct(OC, OC) - pow(radius, 2);

    return ((b*b) - (4 * a * c)) >= 0;  
}

RGB calcRayColor(ray r){
    RGB red = {255, 0, 0};
    RGB blue = {0, 0, 245};

    vec3 circle = {0.5,0.1, 1};
    double radius = 0.2;
    
    if (isIntersectingCircle(circle, radius, r)) 
        return red;
    
    return blue;
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
    

    RGB** buffer = initBuffer(imageWidth, imageHeight);
    for(int i=0; i < imageHeight; i++){
        for(int j=0; j < imageWidth; j++){
            vec3 pixelCentre = {j * deltaU, i * deltaV, focalPoint};
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