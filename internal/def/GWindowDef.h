#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

#include "GSPCore/Graphics/GGI.h"

#include <stdbool.h>

#ifdef _WIN32
    #include <Windows.h>
#endif

#ifdef __unix__

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

    #ifdef __unix__ 
        void* display;
        uintptr_t xDeleteAtom;
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

        VkFramebuffer* vkFramebuffers;

        VkCommandPool vkCommandPool;
        VkCommandBuffer vkCommandBuffer;
    #endif

    #ifdef _WIN32
        HGLRC glContext;
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