#include <stdio.h>
#include <stdlib.h>



struct RGB{
    int red, blue, green;
};

// read and write to the ppm file and functionbailty
int readPPM(char* fileName);
int writePPMPixels(char* fileName, struct RGB* pixels);


int main(){

}