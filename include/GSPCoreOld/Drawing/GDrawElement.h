#ifndef GDRAWELEMENT_H
#define GDRAWELEMENT_H

typedef enum {
    RESIZE_FIT,
    RESIZE_FILL,
    RESIZE_9SLICE
} GDrawElementResizeType;


typedef void* GDrawElement;

GDrawElement GDrawElement_InitFromImage(const char* filePath);


#endif // GDRAWELEMENT_H