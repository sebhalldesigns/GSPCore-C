#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H

#include <stdbool.h>

typedef enum {
    DEFAULT,
    LEGACY_ONLY,
    HIGH_PERFORMANCE_ONLY
} GRenderManagerMode;

void GRenderManager_Init(GRenderManagerMode mode);

bool GRenderManager_TryInitHighPerformance(GRenderManagerMode mode);


void GRenderManager_Cleanup();
#endif // GRENDERMANAGER_H