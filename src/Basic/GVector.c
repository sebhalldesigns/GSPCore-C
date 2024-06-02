#include "GSPCore/Basic/GVector.h"
#include "GSPCore/Debug/GLog.h"

#include <stdlib.h>
#include <string.h>


// initialises vector
GVector GVector_Init() {

    GVector vector = calloc(1, sizeof(GVectorDef));

    if (vector == NULL) {
        DEBUG_LOG(FAIL, "Failed to allocate GVector!");
        return NULL;
    }

    //DEBUG_LOG(INFO, "Allocated GVector at %lu", vector);

    ((GVectorDef*)vector)->data = NULL;
    ((GVectorDef*)vector)->size = 0;

    return vector;
}

// frees vector
void GVector_Free(GVector vector) {

    free(vector);
    //DEBUG_LOG(INFO, "Freed vector from %lu", vector);

}

// adds item to vector if it doesn't exist
void GVector_Add(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    uintptr_t* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    uintptr_t* newData = realloc(data, sizeof(uintptr_t) * (size + 1));

    if (newData == NULL) {
        return;
    }

    newData[size] = (uintptr_t)item;

    ((GVectorDef*)vector)->size++;
    ((GVectorDef*)vector)->data = newData;
}

// removes item from vector if it exists
void GVector_Remove(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    uintptr_t* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    if (size == 1 && data[0] == (uintptr_t)item) {
        // last element in the vector, so free
        free(data);
        ((GVectorDef*)vector)->data = NULL;
        ((GVectorDef*)vector)->size = 0;
        return;
    }

    for (size_t i = 0; i < size; i++) {
        if (data[i] == (uintptr_t)item) {

            size_t itemsToMove = size - (i + 1);
            if (itemsToMove > 0) {
                memcpy(&data[i], &data[i+1], sizeof(uintptr_t) * itemsToMove);
            }

            uintptr_t* newData = realloc(data, sizeof(uintptr_t) * (size - 1));

            if (newData == NULL) {
                DEBUG_LOG(FAIL, "Failed to resize vector at %lu", vector);
                return;
            }

            ((GVectorDef*)vector)->size--;
            ((GVectorDef*)vector)->data = newData;
                
        }
    }

}

// gets size of vector
size_t GVector_Size(GVector vector) {
    
    if (vector == NULL) {
        return 0;
    }

    return ((GVectorDef*)vector)->size;
}

// returns item at index or NULL if the index is invalid.
GVectorItem GVector_Get(GVector vector, size_t index) {

    if (vector == NULL) {
        return NULL;
    }

    if (index >= ((GVectorDef*)vector)->size) {
        return NULL;
    }

    uintptr_t item = ((GVectorDef*)vector)->data[index];
    return (GVectorItem)item;
}

// sets item if it is a valid index
void GVector_Set(GVector vector, size_t index, GVectorItem item) {

    if (vector == NULL) {
        return;
    }

    if (index >= ((GVectorDef*)vector)->size) {
        return;
    }

    ((GVectorDef*)vector)->data[index] = (uintptr_t) item;

}

// returns true if vector contains item, false if it doesn't
bool GVector_Contains(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return false;
    }

    uintptr_t* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        if (data[i] == (uintptr_t)item) {
            return true;
        }
    }

    return false;
}

// returns index of item if it is in vector, or GVector Size (last element + 1) 
// if it is not in the vector.
size_t GVector_IndexOf(GVector vector, GVectorItem item) {

    if (vector == NULL) {
        return SIZE_MAX;
    } 

    uintptr_t* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        if (data[i] == (uintptr_t)item) {
            return i;
        }
    }

    return size + 1;
}

void GVector_Inspect(GVector vector) {

    if (vector == NULL) {
        return;
    } 

    uintptr_t* data = ((GVectorDef*)vector)->data;
    size_t size = ((GVectorDef*)vector)->size;

    for (size_t i = 0; i < size; i++) {
        uintptr_t item = data[i];
        DEBUG_LOG(INFO, "Vector %lu [%d] - %lu", vector, i, item);
    }
}

GVectorItem GVector_Pop(GVector vector) {

    if (vector == NULL) {
        return NULL;
    } 

    GVectorDef* vectorDef = (GVectorDef*)vector;
    if (vectorDef->size < 1) {
        return NULL;
    }

    GVectorItem item = (GVectorItem)vectorDef->data[vectorDef->size - 1];

    uintptr_t* newData = realloc(vectorDef->data, sizeof(uintptr_t) * (vectorDef->size - 1));

    if (newData == NULL) {
        DEBUG_LOG(FAIL, "Failed to resize vector at %lu", vector);
        return NULL;
    }

    vectorDef->size--;
    vectorDef->data = newData;

    return item;

}