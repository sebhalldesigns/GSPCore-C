#ifndef GCAIRODDRAWINGMANAGER_H
#define GCAIRODDRAWINGMANAGER_H

#include <stdbool.h>

bool GCairoDrawingManager_Init();
bool GCairoDrawingManager_SetupView(GView* view);
void GCairoDrawingManager_DrawView(GView* view);

#endif // GCAIRODDRAWINGMANAGER_H