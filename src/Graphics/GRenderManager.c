#include <GSPCore/GSPCore.h>
#include <GSPCore/Graphics/GRenderManager.h>

#ifdef GSPCORE_BUILD_UNIX
#include <GSPCore/Graphics/Backends/Vulkan/GVulkanRenderManager.h>
#elif GSPCORE_BUILD_WIN32
#include <GSPCore/Graphics/Backends/Vulkan/GVulkanRenderManager.h>
#elif GSPCORE_BUILD_WEB
#include <GSPCore/Graphics/Backends/WebGL/GWebGlRenderManager.h>

#endif

static GRenderManagerBackend backend = BACKEND_NONE;

bool GRenderManager_Init() {
        
    GApplicationEnvironment environment = GApplication_GetEnvironment();

    switch (environment) {
    #ifdef GSPCORE_BUILD_UNIX

        case ENVIRONMENT_UNIX:
            
            if (GVulkanRenderManager_Init()) {
                backend = BACKEND_VULKAN;
                return true;
            }
    #elif GSPCORE_BUILD_WIN32

         case ENVIRONMENT_WIN32:
            
            if (GVulkanRenderManager_Init()) {
                printf("DONE RENDER MANAGER INIT\n");
                backend = BACKEND_VULKAN;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WEB
        case ENVIRONMENT_WEB:
            
            if (GWebGlRenderManager_Init()) {
                printf("DONE RENDER MANAGER INIT\n");
                backend = BACKEND_WEBGL;
                return true;
            }

            return false;
    #endif 
        default:
            return false;
    }


}

bool GRenderManager_SetupWindow(GWindow* window) {
    

    switch (backend) {
    #ifdef GSPCORE_BUILD_UNIX
        case BACKEND_VULKAN:
            return GVulkanRenderManager_SetupWindow(window);
    #elif GSPCORE_BUILD_WIN32
        case BACKEND_VULKAN:
            return GVulkanRenderManager_SetupWindow(window);
    #elif GSPCORE_BUILD_WEB
        case BACKEND_WEBGL:
            return GWebGlRenderManager_SetupWindow(window);
    #endif
        default:
            return false;
    }
}

void GRenderManager_RenderWindow(GWindow* window) {
    switch (backend) {
    #ifdef GSPCORE_BUILD_UNIX
        case BACKEND_VULKAN:
            return GVulkanRenderManager_RenderWindow(window);
    #elif GSPCORE_BUILD_WIN32
        case BACKEND_VULKAN:
            return GVulkanRenderManager_RenderWindow(window);
    #elif GSPCORE_BUILD_WEB
        case BACKEND_WEBGL:
            return GWebGlRenderManager_RenderWindow(window);
    #endif

        default:
            return;
    }
}