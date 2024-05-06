#ifndef GVECTORDEF_H
#define GVECTORDEF_H

#include "internal/include/GVector.h"

typedef struct {
    uintptr_t* data;
    size_t size;
} GVectorDef;

#endif // GVECTORDEF_H