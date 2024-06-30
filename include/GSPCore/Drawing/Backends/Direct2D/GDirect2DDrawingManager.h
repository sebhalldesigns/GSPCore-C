#ifndef GDIRECT2DDRAWINGMANAGER_H
#define GDIRECT2DDRAWINGMANAGER_H

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>

bool GDirect2DDrawingManager_Init();
bool GDirect2DDrawingManager_SetupView(GView* view);
void GDirect2DDrawingManager_DrawView(GView* view);

#endif // GDIRECT2DDRAWINGMANAGER_H