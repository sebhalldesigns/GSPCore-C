#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "../include/GSPCore/GMemoryPool.h"

#define autofree __attribute__((cleanup(free_stack)))

__attribute__ ((always_inline))
inline void free_stack(void *ptr) {
    //free(*(void **) ptr);
    printf("Thing being deallocated!\n");
}

typedef struct {
    const char* message;
} Object;

#define GObject __attribute__((cleanup(free_object))) Object

__attribute__ ((always_inline))
inline void free_object(Object* object) {
    //free(*(void **) ptr);
    printf("Object being deallocated! %s\n", object->message);
}



GHandle GMemoryPool_Alloc(size_t size) {

    if (initialPool == NULL) {
        printf("ALLOCATING POOL\n");
        initialPool = calloc(memoryPoolSize, handleSize);
        printf("allocated %lu bytes\n", memoryPoolSize * handleSize);

        // set each entry pointing to it's neigbour
        for(size_t i = 0; i < (memoryPoolSize - 1); i++) {
            initialPool->data[i] = i;
        }

        // set the last item to itself
        initialPool->data[memoryPoolSize - 1] = memoryPoolSize - 1;
    }

    

    size_t nextFree = initialPool->firstFree;

    // this loop is not tested
    // ultimately it wants to iterate over the free data until it finds a set of 
    // contiguous free slots the size of the data, or move on to another memory pool.
    
    for (int i = 0; i < size; i++) {

        if (nextFree + size >= memoryPoolSize - 1) {
            // this will overflow so go to the next pool
            return SIZE_MAX; // SIZE_MAX always invalid
        }


        if (initialPool->data[nextFree + i] == nextFree + i) {
            // all good
        } else {
            // not all good so reset nextFree
            nextFree = initialPool->data[nextFree];
            i = 0;
        }
    }

    return 0;
}




int main() {

  
    GHandle handle = GMemoryPool_Alloc(10);

    return 0;
}