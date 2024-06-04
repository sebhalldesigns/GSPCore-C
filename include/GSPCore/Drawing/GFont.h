#ifndef GFONT_H
#define GFONT_H

#include <string.h>

#include "../Types/GSPCoreOpaqueTypes.h"

typedef struct {
    const char* fontFile;
    float size;
} GFontInfo;

GFont GFont_Init(GFontInfo info);

void GFont_Free(GFont font);


#endif // GFONT_H