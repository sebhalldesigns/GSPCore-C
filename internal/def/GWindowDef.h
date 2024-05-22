#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

#include "GSPCore/Graphics/GGI.h"

#include <stdbool.h>

#ifdef EMSCRIPTEN

#elif  _WIN32
    #include <Windows.h>
     #include <vulkan/vulkan.h>

#elif __unix__

    #include <vulkan/vulkan.h>
#endif

typedef struct {
    
    void* userData;

    uintptr_t rawHandle;
    int width;
    int height;
    const char* title;
    GWindowDrawDelegate drawDelegate;
    GWindowCloseDelegate closeDelegate;
    GWindowWillCloseDelegate willCloseDelegate;
    GWindowGraphicsInterface graphicsInterface;
    GWindowResizeDelegate resizeDelegate;
    GWindowPointerMoveDelegate pointerMoveDelegate;
    GWindowButtonDownDelegate buttonDownDelegate;
    GWindowButtonUpDelegate buttonUpDelegate;
    
    #ifdef EMSCRIPTEN
    
    #elif  _WIN32
           void* hinstance;
    #elif __unix__ 
        void* display;
        uintptr_t xDeleteAtom;
    #endif

    #ifdef EMSCRIPTEN

    #elif defined _WIN32 || defined __unix__
        VkSurfaceKHR vkSurface;

        VkDevice vkDevice;
        uint32_t graphicsQueueFamily;
        uint32_t presentQueueFamily;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR vkSwapchain;
        VkFormat vkSwapchainImageFormat;
        VkExtent2D vkSwapchainExtent;

        uint32_t vkImageCount;
        VkImage* vkImages;
        VkImageView* vkImageViews;

        // possibly should be per-window rather than global
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderingFinishedSemaphore;
        VkFence inFlightFence;

        VkRenderPass vkRenderPass;
        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkPipeline;

        size_t vkFramebufferCount;
        VkFramebuffer* vkFramebuffers; 

        VkCommandPool vkCommandPool;
        VkCommandBuffer vkCommandBuffer;
    #endif



    GShader shader;
    GTexture texture;
    GVertexBuffer vertexBuffer;
    bool redrawFlag;
} GWindowDef;

// called once by system
bool GWindowDef_Init();
void GWindowDef_Poll();
size_t GWindowDef_NumberOfOpenWindows();

#endif // GWINDOWDEF_H