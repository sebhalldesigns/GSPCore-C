#ifndef GMEMORYPOOL_H
#define GMEMORYPOOL_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

// designed for 32 or 64 bit systems

// implement using a linked-list system. https://arxiv.org/pdf/2210.16471

// smart pointers: https://snai.pe/posts/c-smart-pointers 


// RAII

const size_t handleSize = sizeof(size_t);
const size_t memoryPoolSize = __UINT16_MAX__;

typedef size_t GHandle;

typedef struct {
    size_t poolId;
    void* nextPool;
    size_t firstFree;
    size_t data[__UINT16_MAX__];
} GMemoryPool;

static GMemoryPool* initialPool = NULL;





#endif // GMEMORYPOOL_H