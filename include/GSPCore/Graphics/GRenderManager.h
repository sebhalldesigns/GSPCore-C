#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>

#include <stdbool.h>



typedef enum {
    BACKEND_NONE,
    BACKEND_VULKAN,
    BACKEND_METAL,
    BACKEND_WEBGL
} GRenderManagerBackend;

bool GRenderManager_Init();
bool GRenderManager_SetupWindow(GWindow* window);
void GRenderManager_RenderWindow(GWindow* window);



#endif // GRENDERMANAGER_H