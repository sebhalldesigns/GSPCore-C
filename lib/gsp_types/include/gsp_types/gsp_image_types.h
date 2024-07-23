#ifndef GSP_IMAGE_TYPES_H
#define GSP_IMAGE_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef uintptr_t gdrawing_t;

typedef struct {
    size_t width;
    size_t height;
    uint8_t* data;
} gbitmap_t;



#endif // GSP_IMAGE_TYPES_H