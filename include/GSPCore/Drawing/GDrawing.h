#ifndef GDRAWING_H
#define GDRAWING_H

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <GSPCore/Types/GSPCoreGeometry.h>

#include <stdint.h>
#include <stddef.h>

typedef enum {
    DRAW_COMMAND_NONE,
    DRAW_COMMAND_SET_FILL_COLOR,
    DRAW_COMMAND_SET_FILL_GRADIENT,
    DRAW_COMMAND_SET_FILL_IMAGE,
    DRAW_COMMAND_SET_STROKE_COLOR,
    DRAW_COMMAND_SET_STROKE_WIDTH,
    DRAW_COMMAND_SET_STROKE_GRADIENT,
    DRAW_COMMAND_SET_STROKE_IMAGE,
    DRAW_COMMAND_SET_STROKE_DASH,
    DRAW_COMMAND_SET_STROKE_JOINT,
    DRAW_COMMAND_SET_STROKE_TERMINATION,
    DRAW_COMMAND_SET_SHADOW_COLOR,
    DRAW_COMMAND_SET_SHADOW_OFFSET,
    DRAW_COMMAND_SET_SHADOW_BLUR,
    
    DRAW_COMMAND_TRANSFORM_RESET,
    DRAW_COMMAND_TRANSFORM_ROTATE, 
    DRAW_COMMAND_TRANSFORM_SCALE,
    DRAW_COMMAND_TRANSFORM_TRANSLATE,
    
    DRAW_COMMAND_PATH_BEGIN,
    DRAW_COMMAND_PATH_END,
    DRAW_COMMAND_PATH_LINE,
    DRAW_COMMAND_PATH_ARC,
    DRAW_COMMAND_PATH_CONIC,
    DRAW_COMMAND_PATH_QUADRATIC,
    DRAW_COMMAND_PATH_CUBIC,

    DRAW_COMMAND_GEOMETRY_LINE,
    DRAW_COMMAND_GEOMETRY_ARC,
    DRAW_COMMAND_GEOMETRY_QUADRATIC,
    DRAW_COMMAND_GEOMETRY_CUBIC,
    DRAW_COMMAND_GEOMETRY_RECTANGLE,
    DRAW_COMMAND_GEOMETRY_ROUNDED,
    DRAW_COMMAND_GEOMETRY_ELLIPSE,
    DRAW_COMMAND_GEOMETRY_TEXT

} GDrawCommandOperation;

typedef union {
    struct {
        GPoint location;  
    } moveToParameters;

} GDrawCommandParameters;

typedef struct {
    GDrawCommandOperation operation;
    GDrawCommandParameters parameters;
} GDrawCommand;

struct GDrawing {
    GSize size;

    size_t commandCapacity;
    size_t commandCount;
    GDrawCommand* commandList;
    


    size_t allocatedWidth;
    size_t allocatedHeight;
    uint8_t* rawData;
};

#endif // GDRAWING_H