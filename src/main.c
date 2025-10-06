#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#define XSIZE 800
#define YSIZE 800

#define RGBMAX 255


struct RGB{
    uint8_t r, b, g;
};

// read and write to the ppm file and functionbailty
//int readPPM(FILE* f);
void writePPMPixels(FILE* f, struct RGB* pixels, const int size);
void initPPMHeader(FILE* f, int n, int m);

int openPPMFile(FILE** f, const char* fname);

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

void writePPMPixels(FILE* f, struct RGB* pixels, const int size){
    fwrite(pixels, size / sizeof(struct RGB), size, f);
}


int main(){
    const int size = XSIZE * YSIZE;
    
    struct RGB* pixels[YSIZE];
    pixels[0] = (struct RGB*)malloc(XSIZE * YSIZE * sizeof(struct RGB));

    for(int i=1; i < YSIZE; i++){
        pixels[i] = pixels[0] + (i * XSIZE);
    }

    for(int i=0; i < YSIZE; i++){
        for(int j=0; j < XSIZE; j++){
            int r = rand() % RGBMAX;
            int b = rand() % RGBMAX;
            int g = rand() % RGBMAX;

            struct RGB pixel ={r, b, g };
            pixels[i][j] = pixel;            
        }
    }
    
    FILE* f = NULL;
    int result = openPPMFile(&f, "test.ppm");
    
    if(result == -1)
        return result; 
    
    initPPMHeader(f, XSIZE, YSIZE);
    
    for(int i=0; i < YSIZE; i++)
        writePPMPixels(f, pixels[i], XSIZE);

    free(pixels[0]);
    fclose(f);
}