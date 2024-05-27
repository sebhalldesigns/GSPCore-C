#ifndef GVIEW_H
#define GVIEW_H

#include <stddef.h>
#include <stdbool.h>

// UPoint defines a simple point in view space
typedef struct {
    float x;
    float y;
} GPoint;

// URect defines a simple rectangular region in view space
typedef struct {
    float x;
    float y;
    float width;
    float height;
} GRect;

typedef struct {
    float x;
    float y;
} GScale;

typedef struct {
    float x;
    float y;
} GTranslation;

typedef struct {
    float degrees;
} GRotation;

typedef struct {
    GTranslation translation;
    GRotation rotation;
    GScale scale;
} GTransform;

typedef struct {
    float red;
    float green;
    float blue;
    float alpha;
} GColor;

typedef enum {
    GVIEWLAYOUT_NONE,
    
    

    // for fixed element width and height
    GVIEWLAYOUT_FITWRAP,
    
    // for elements with fixed min width, can expand
    GVIEWLAYOUT_FILLWRAP,

    

} GViewLayout;


typedef struct {
    GRect frame;
    GColor color;
} GViewInfo;

typedef void* GView;


GView GView_Alloc(GViewInfo info);

void GView_Free();




#endif // GVIEW_H