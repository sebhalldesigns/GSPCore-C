#ifndef GVERTEXBUFFERDEF_H
#define GVERTEXBUFFERDEF_H

#include "GSPCore/Graphics/GVertexBuffer.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    float* vertexData;
    size_t vetexDataLength;
    uint32_t* indexData;
    size_t indexDataLength;
    uint32_t glVertexArrayBuffer;
    uint32_t glVertexBuffer;
    uint32_t glElementBuffer;
} GVertexBufferDef;

#endif // GVERTEXBUFFERDEF_H