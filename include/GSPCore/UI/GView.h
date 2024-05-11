#ifndef GVIEW_H
#define GVIEW_H

#include <stddef.h>
#include <stdbool.h>

typedef void* GView;


GView GView_Alloc();

void GView_Free();

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

#endif // GVIEW_H