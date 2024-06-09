#ifndef GDRAWING_H
#define GDRAWING_H

// GDrawing represents a generic 'drawing' that can contain
// vector graphics and text. It is indended to provide the 
// basis for rendering views and their contents in GSPCore,
// but could be generalised for other 2D drawing purposes.
//
// Initially, GDrawing aims to simply convert paths and shapes
// into sets of lines and polygons to be rendered by OpenGL,
// Metal and Vulkan. This should be a relatively trivial process
// and should be quite efficient.

/*

want to somehow make the drawing stages modular?
but this will likely require more drawing stages althogh it doesnt necessarily have to.
Possibly let's use instances of uint32_t to be generic structures as can make the drawing
system more like an instruction set than a fixed struct!

*/

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"
#include "../Types/GSPCoreGraphics.h"
#include <stddef.h>

GDrawing GDrawing_Init();

void GDrawing_Free(GDrawing drawing);

typedef struct {
    size_t numElements;
} GDrawingDef;

typedef enum {
    // TEXT & IMAGE
    ELEMENT_TYPE_TEXT,
    ELEMENT_TYPE_IMAGE,
    
    // SHAPES
    ELEMENT_TYPE_CIRCLE,
    ELEMENT_TYPE_RECT,
    ELEMENT_TYPE_POLYGON
} GDrawingElementType;


typedef enum {
    FILL_STYLE_NONE,
    FILL_STYLE_SOLID,
    FILL_STYLE_CARTESIAN_GRADIENT,
    FILL_STYLE_POLAR_GRADIENT,
    FILL_STYLE_IMAGE
} GDrawingElementFillStyle;

typedef struct {
    GDrawingElementType elementType;
    

} GDrawingElement;


#endif // GDRAWING_H