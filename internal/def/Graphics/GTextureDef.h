#ifndef GTEXTUREDEF_H
#define GTEXTUREDEF_H

#include "GSPCore/Graphics/GTexture.h"

#include <stdint.h>

typedef struct {
    int width;
    int height;
    uint32_t glBuffer;
} GTextureDef;


#endif // GTEXTUREDEF_H