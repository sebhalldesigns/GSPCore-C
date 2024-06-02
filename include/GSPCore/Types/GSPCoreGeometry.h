#ifndef GSPCOREGEOMETRY_H
#define GSPCOREGEOMETRY_H

#include <stdbool.h>

typedef float GFloat;

typedef struct {
    GFloat width;
    GFloat height;
} GSize;

typedef struct {
    GFloat x;
    GFloat y;
} GPoint;

typedef struct {
    GSize size;
    GPoint origin;
} GRect;

bool GRect_ContainsPoint(GRect rect, GPoint point);

typedef struct {
    GFloat left;
    GFloat top;
    GFloat right;
    GFloat bottom;
} GThickness;

typedef enum {
    ALIGNMENT_BEGINNING,
    ALIGNMENT_CENTER,
    ALIGNMENT_END,
    ALIGNMENT_FILL
} GAlignment;

#endif // GSPCOREGEOMETRY_H