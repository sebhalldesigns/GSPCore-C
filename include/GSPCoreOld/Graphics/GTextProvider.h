#ifndef GTEXTPROVIDER_H
#define GTEXTPROVIDER_H

// The GTextProvider class is an abstraction over text rendering and layout APIs. 
// Under normal conditions, it will provide text of the same high quality found on
// any native application. 

#include "GTexture.h"

typedef void* GTextProvider;

typedef struct {
    float width;
    float height;
    float offsetX;
    float offsetY;
    float nextX;
    float nextY;
} GTextMeasurements;

GTextProvider GTextProvider_Alloc(char* typeface, float size);
void GTextProvider_Free(GTextProvider textProvider);

// return measurements. useful for calculating text wrapping etc.
GTextMeasurements GTextProvider_MeasureText(GTextProvider textProvider, char* text);

// measure and render text
GTexture GTextProvider_Render(GTextProvider textProvider, char* text);





#endif // GTEXTPROVIDER_H