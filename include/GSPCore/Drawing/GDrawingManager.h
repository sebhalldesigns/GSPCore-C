#ifndef GDRAWINGMANAGER_H
#define GDRAWINGMANAGER_H

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>

#include "GDrawing.h"

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
bool GDrawingManager_SetupView(GView* view);
void GDrawingManager_DrawView(GView* view);


#endif // GDRAWINGMANAGER_H