#ifndef GFRAMEDEF_H
#define GFRAMEDEF_H

#include "GSPCore/Graphics/GFrame.h"

#include <stdint.h>

typedef struct {
    int width;
    int height;
    uint32_t glBuffer;
} GFrameDef;

#endif // GFRAMEDEF_H
