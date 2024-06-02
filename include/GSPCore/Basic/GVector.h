#ifndef GVECTOR_H
#define GVECTOR_H

// GVector - a lightweight, dynamically resizable vector container for pointer types.
// It is naturally zero-indexed

#include "../Types/GSPCoreOpaqueTypes.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// initialises vector
GVector GVector_Init();

// frees vector
void GVector_Free(GVector vector);

// adds item to vector if it doesn't exist
void GVector_Add(GVector vector, GVectorItem item);

// removes item from vector if it exists
void GVector_Remove(GVector vector, GVectorItem item);

// gets size of vector, returns NULL if invalid.
size_t GVector_Size(GVector vector);

// returns item at index or NULL if the index is invalid.
GVectorItem GVector_Get(GVector vector, size_t index);

// sets item if it is a valid index
void GVector_Set(GVector vector, size_t index, GVectorItem item);

// returns true if vector contains item, false if it doesn't
bool GVector_Contains(GVector vector, GVectorItem item);

// returns index of item if it is in vector, or GVector Size (last element + 1) 
// if it is not in the vector.
size_t GVector_IndexOf(GVector vector, GVectorItem item);

// logs contents of vector to console. Requires debug
void GVector_Inspect(GVector vector);

// removes and returns last element of vector or NULL if the vector was empty.
GVectorItem GVector_Pop(GVector vector);


#ifdef GSPCORE_BUILD

typedef struct {
    uintptr_t* data;
    size_t size;
} GVectorDef;

#endif 

#endif // GVECTOR_H