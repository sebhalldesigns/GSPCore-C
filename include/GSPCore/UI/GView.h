#ifndef GVIEW_H
#define GVIEW_H

#include <stddef.h>
#include <stdbool.h>

// UPoint defines a simple point in view space
typedef struct {
    float x;
    float y;
} UPoint;

// URect defines a simple rectangular region in view space
typedef struct {
    float x;
    float y;
    float width;
    float height;
} URect;

typedef struct {
    float x;
    float y;
} UScale;

typedef struct {
    float x;
    float y;
} UTranslation;

typedef struct {
    float degrees;
} URotation;

typedef struct {
    UTranslation translation;
    URotation rotation;
    UScale scale;
} UTransform;

typedef struct {
    float red;
    float green;
    float blue;
    float alpha;
} UColor;

typedef enum {
    GVIEWLAYOUT_NONE,
    
    

    // for fixed element width and height
    GVIEWLAYOUT_FITWRAP,
    
    // for elements with fixed min width, can expand
    GVIEWLAYOUT_FILLWRAP,

    

} GViewLayout;


typedef struct {
    URect frame;
    UColor color;
} GViewInfo;

typedef void* GView;


GView GView_Alloc(GViewInfo info);

void GView_Free();




#endif // GVIEW_H