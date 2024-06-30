#ifndef GCAIRODDRAWINGMANAGER_H
#define GCAIRODDRAWINGMANAGER_H

#include <GSPCore/Drawing/GDrawing.h>
#include <stdbool.h>

bool GCairoDrawingManager_Init();
bool GCairoDrawingManager_SetupDrawing(GDrawing* drawing);
void GCairoDrawingManager_RenderDrawing(GDrawing* drawing);

#endif // GCAIRODDRAWINGMANAGER_H