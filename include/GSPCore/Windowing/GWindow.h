#ifndef GWINDOW_H
#define GWINDOW_H

// GWindow.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GWindow is a generic top-level application window

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"

#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan.h>
#include <wayland-client.h>
#include <X11/Xlib.h>

typedef struct {
    // Wayland handles
    struct wl_display* wl_display;
    struct wl_surface* wl_surface;
    struct xdg_surface* xd_surface;
    struct xdg_toplevel* xd_toplevel;

    // X11 handles
    Display* xDisplay;
    Window xWindow;
    Atom xDeleteAtom;

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

} GWindowPlatformHandles;

struct GWindow{
    //GWindowInfo info;
    //GWindowController controller;
    //GView rootView;
    GRect frame;
    const char* title;
    GWindow* nextWindow;
    GWindowPlatformHandles platformHandles;
};





#endif // GWINDOW_H