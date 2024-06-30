#ifndef GDIRECT2DDRAWINGMANAGER_H
#define GDIRECT2DDRAWINGMANAGER_H

#include <GSPCore/Drawing/GDrawing.h>
#include <stdbool.h>

bool GDirect2DDrawingManager_Init();
bool GDirect2DDrawingManager_SetupDrawing(GDrawing* drawing);
void GDirect2DDrawingManager_RenderDrawing(GDrawing* drawing);

#endif // GDIRECT2DDRAWINGMANAGER_H