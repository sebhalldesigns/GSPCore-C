#ifndef GVERTEXBUFFER_H
#define GVERTEXBUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef void* GVertexBuffer;

GVertexBuffer GVertexBuffer_Alloc(float* vertexData, size_t vertexDataLength, uint32_t* indexData, size_t indexDataLength);
void GVertexBuffer_Free(GVertexBuffer buffer);

#endif // GVERTEXBUFFER_H