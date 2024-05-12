#ifndef HIPERRENDERMANAGER_H
#define HIPERRENDERMANAGER_H

#include <stdbool.h>

#include "GRenderManager.h"
#include "GWindow.h"


bool GHiperRenderManager_TrySetup(GRenderManagerMode mode);

bool GHiperRenderManager_TrySetupWindow(GWindow window);

void GHiperRenderManager_CleanupWindow(GWindow window);

void GHiperRenderManager_Cleanup();

#endif // HIPERRENDERMANAGER_H