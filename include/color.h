#ifndef COLOR
#define COLOR

#include <stdint.h>

#define RGBMAX 255

typedef struct {
    uint8_t r, g, b;
}RGB;

RGB applyBrightness(RGB, double);

#endif