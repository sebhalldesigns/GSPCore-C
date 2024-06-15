#include <GSPCore/GSPCore.h>
#include <GSPCore/Graphics/GRenderManager.h>
#include <GSPCore/Graphics/Backends/Vulkan/GVulkanRenderManager.h>

static GRenderManagerBackend backend = BACKEND_NONE;

bool GRenderManager_Init() {
        
    GApplicationEnvironment environment = GApplication_GetEnvironment();

    switch (environment) {
        case ENVIRONMENT_UNIX:
            
            if (GVulkanRenderManager_Init()) {
                backend = BACKEND_VULKAN;
                return true;
            }

         case ENVIRONMENT_WIN32:
            
            if (GVulkanRenderManager_Init()) {
                printf("DONE RENDER MANAGER INIT\n");
                backend = BACKEND_VULKAN;
                return true;
            }

            return false;
        default:
            return false;
    }


}

bool GRenderManager_SetupWindow(GWindow* window) {
    

    switch (backend) {
        case BACKEND_VULKAN:
            return GVulkanRenderManager_SetupWindow(window);
        default:
            return false;
    }
}

void GRenderManager_RenderWindow(GWindow* window) {
    switch (backend) {
        case BACKEND_VULKAN:
            return GVulkanRenderManager_RenderWindow(window);
        default:
            return;
    }
}