#ifndef GSPCOREGRAPHICS_H
#define GSPCOREGRAPHICS_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    float red;
    float green;
    float blue;
    float alpha;
} GColor;

typedef struct {
    uint32_t* rawData;
    size_t width;
    size_t height;
} GRawImage;

// IGNORE THIS FOR NOW
// WANT TO MAKE A CONDITIONAL STYLING SYSTEM
// LIKE CSS BUT DEFINED IN C
// SO THAT THINGS LIKE LIGHT/DARK MODE AND VIEWPORT SIZE CAN BE CONDITIONAL

/*typedef struct {
    GColor lightColor;
    GColor darkColor;
} GAdaptiveColor;

const static GAdaptiveColor GColor_SystemBackground = (GAdaptiveColor) { 
    (GColor) { 0.9f, 0.9f, 0.9f, 1.0f },
    (GColor) { 0.1f, 0.1f, 0.1f, 1.0f }
};*/


#endif // GSPCOREGRAPHICS_H