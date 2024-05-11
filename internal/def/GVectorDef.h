#ifndef GVECTORDEF_H
#define GVECTORDEF_H

#include "GSPCore/GVector.h"

typedef struct {
    uintptr_t* data;
    size_t size;
} GVectorDef;

#endif // GVECTORDEF_H