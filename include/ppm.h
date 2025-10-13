#ifndef PPM
#define PPM

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "color.h"

// read and write to the ppm file and functionbailty
//int readPPM(FILE* f);
void writePPMPixels(FILE* f, RGB* pixels, const int size);
void initPPMHeader(FILE* f, int n, int m);
int openPPMFile(FILE** f, const char* fname);

RGB** initBuffer(int xSize, int ySize);
void writeBufferToPPM(RGB **buffer, FILE* f, int xSize, int ySize);

#endif