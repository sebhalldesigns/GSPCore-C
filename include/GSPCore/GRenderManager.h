#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H

#include <stdbool.h>


#include "GWindow.h"

typedef enum {
    DEFAULT,
    LEGACY_ONLY,
    HIGH_PERFORMANCE_ONLY
} GRenderManagerMode;

void GRenderManager_Setup(GRenderManagerMode mode);

void GRenderManager_SetupWindow(GWindow window);


void GRenderManager_CleanupWindow(GWindow window);

void GRenderManager_Cleanup();


#endif // GRENDERMANAGER_H