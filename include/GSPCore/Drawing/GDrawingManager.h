#ifndef GDRAWINGMANAGER_H
#define GDRAWINGMANAGER_H

#include <GSPCore/Drawing/GDrawing.h>
#include <stdbool.h>

// GSPCore - GDrawingManager.h
// This is an abstraction layer for rendering vector graphics.

typedef enum {
    DRAWING_BACKEND_NONE,
    DRAWING_BACKEND_CAIRO,
    DRAWING_BACKEND_DIRECT2D,
    DRAWING_BACKEND_COREGRAPHICS,
    DRAWING_BACKEND_CANVAS
} GDrawingManagerBackend;

bool GDrawingManager_Init();
bool GDrawingManager_SetupDrawing(GDrawing* drawing);
void GDrawingManager_RenderDrawing(GDrawing* drawing);


#endif // GDRAWINGMANAGER_H