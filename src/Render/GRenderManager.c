#include <GSPCore/GSPCore.h>
#include <GSPCore/Render/GRenderManager.h>

#ifdef GSPCORE_BUILD_UNIX
#include <GSPCore/Render/Backends/Vulkan/GVulkanRenderManager.h>
#elif GSPCORE_BUILD_WIN32
#include <GSPCore/Render/Backends/Vulkan/GVulkanRenderManager.h>
#elif GSPCORE_BUILD_MACOS
#include <GSPCore/Render/Backends/Metal/GMetalRenderManager.h>
#elif GSPCORE_BUILD_WEB
#include <GSPCore/Render/Backends/WebGL/GWebGlRenderManager.h>

#endif

static GRenderManagerBackend backend = BACKEND_NONE;

bool GRenderManager_Init() {
        
    GApplicationEnvironment environment = GApplicationManager_GetEnvironment();

    switch (environment) {
    #ifdef GSPCORE_BUILD_UNIX
        case ENVIRONMENT_UNIX:
            
            if (GVulkanRenderManager_Init()) {
                backend = BACKEND_VULKAN;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WIN32
         case ENVIRONMENT_WIN32:
            
            if (GVulkanRenderManager_Init()) {
                backend = BACKEND_VULKAN;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_MACOS
         case ENVIRONMENT_MACOS:
            
            if (GMetalRenderManager_Init()) {
                backend = BACKEND_METAL;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WEB
        case ENVIRONMENT_WEB:
            
            if (GWebGlRenderManager_Init()) {
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
    #elif GSPCORE_BUILD_MACOS
        case BACKEND_METAL:
            return GMetalRenderManager_SetupWindow(window);
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
    #elif GSPCORE_BUILD_MACOS
        case BACKEND_METAL:
            return GMetalRenderManager_RenderWindow(window);
    #elif GSPCORE_BUILD_WEB
        case BACKEND_WEBGL:
            return GWebGlRenderManager_RenderWindow(window);
    #endif

        default:
            return;
    }
}