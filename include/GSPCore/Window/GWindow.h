#ifndef GWINDOW_H
#define GWINDOW_H

// GWindow.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GWindow is a generic top-level application window

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"

#ifdef GSPCORE_BUILD_UNIX

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

#elif GSPCORE_BUILD_WIN32

    #define VK_USE_PLATFORM_WIN32_KHR
    #include <vulkan/vulkan.h>

    #include <Windows.h>
    
    typedef struct {
        HINSTANCE instance;
        HWND handle;

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

#elif GSPCORE_BUILD_MACOS

    #import <Cocoa/Cocoa.h>
    #import <Metal/Metal.h>
    #import <MetalKit/MetalKit.h>

    @interface CocoaWindow : NSWindow
    @property GWindow* window;
    @end

    @interface CocoaMetalView: MTKView
    @property GWindow mywindow;
    @property id<MTLCommandQueue> commandQueue;
    @end

    typedef struct {
        CocoaWindow* window;
        CocoaMetalView* metalView;
        id<MTLDevice> mtlDevice;
        id<MTLRenderPipelineState> mtlPipelineState;
    } GWindowPlatformHandles;
    
#elif GSPCORE_BUILD_WEB

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

 typedef struct {
    GLuint framebuffer;
} GWindowPlatformHandles;

#endif

struct GWindow{
    //GWindowInfo info;
    //GWindowController controller;
    //GView rootView;
    GRect frame;
    const char* title;
    
    GWindow* nextWindow;
    GView* rootView;

    GWindowPlatformHandles platformHandles;
};





#endif // GWINDOW_H