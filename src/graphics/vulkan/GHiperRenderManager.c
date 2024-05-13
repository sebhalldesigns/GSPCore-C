// https://github.com/seb-hall/UniversalUI/blob/5bcbce94ba820519da9586a627d0409d2ebd115a/Rendering/source/uRenderManager.cpp

#include "GSPCore/GHiperRenderManager.h"
#include "GSPCore/GLog.h"

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#ifdef __unix__
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>

#include "internal/def/GWindowDef.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __unix__
#include <dlfcn.h>
#endif


void* vulkanLibrary = NULL;

#ifdef DEBUG
    bool ENABLE_VULKAN_DEBUG = true;
#else
    bool ENABLE_VULKAN_DEBUG = false;
#endif

VkInstance vkInstance = NULL;
VkDebugUtilsMessengerEXT vkDebugMessenger = NULL;
VkPhysicalDevice vkPhysicalDevice;



typedef struct {
    const char* name;
    bool supported;
} VulkanDependency;

VulkanDependency validationLayers[] = {
    { "VK_LAYER_KHRONOS_validation", false },
};

const char* validationLayerNames[] = {
    "VK_LAYER_KHRONOS_validation",
};

VulkanDependency extensionDependencies[] = {
    { "VK_KHR_surface", false },
    #ifdef _WIN32
    { "VK_KHR_win32_surface", false },
    #endif

    #ifdef __unix__
    { "VK_KHR_wayland_surface", false },
    { "VK_KHR_xlib_surface", false },
    #endif

    #ifdef DEBUG
     { "VK_EXT_debug_report", false },
    { "VK_EXT_debug_utils", false },
    #endif
};


const char* dependencyNames[] = {
    "VK_KHR_surface", 
    #ifdef _WIN32
    "VK_KHR_win32_surface",
    #endif

    #ifdef __unix__
    "VK_KHR_wayland_surface",
    "VK_KHR_xlib_surface",
    #endif

    #ifdef DEBUG
       "VK_EXT_debug_report",
       "VK_EXT_debug_utils",
    #endif
};

const uint32_t numValidationlayers = sizeof(validationLayers) / sizeof(VulkanDependency);
const uint32_t numExtensionDependencies = sizeof(extensionDependencies) / sizeof(VulkanDependency);

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    GLog(WARNING, "Vulkan Debug: %s", pCallbackData->pMessage);

    return VK_FALSE;
}

// MARK: TRY SET UP WINDOW
bool GHiperRenderManager_TrySetup(GRenderManagerMode mode) {
    GLog(INFO, "Starting Hiper Render Manager...");

    // try to load the vulkan library
    #if _WIN32
        vulkanLibrary = LoadLibrary( "vulkan-1.dll" );
    #elif __unix__
        vulkanLibrary = dlopen( "libvulkan.so",  RTLD_NOW | RTLD_GLOBAL );
    #endif

    if (vulkanLibrary == NULL) {
        GLog(WARNING, "Vulkan library not found. Using legacy rendering mode.");
        return false;
    } 
    
    GLog(INFO, "Vulkan library loaded");

    // MARK: CHECK FOR VALIDATION LAYERS
    
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    // ALLOC @layerProperties
    VkLayerProperties* layerProperties = calloc(layerCount, sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties);

    printf("\tFound debug layers:\n");
    for (int i = 0; i < layerCount; i++) {
        printf("\t\t%s\n", layerProperties[i].layerName);

        // set the validation layer dependency found flag to true
        for (int j = 0; j < numValidationlayers; j++) {
            if (strcmp(validationLayers[j].name, layerProperties[i].layerName) == 0) {
                validationLayers[j].supported = true;
            }
        }
    }

    // FREE @layerProperties
    free(layerProperties);

    if (ENABLE_VULKAN_DEBUG) {
        // check all validation layer dependencies are met
        for (int i = 0; i < numValidationlayers; i++) { 
            if (!validationLayers[i].supported) {
                GLog(WARNING, "Vulkan validation layer '%s' not supported. Disabling Vulkan debug.", validationLayers[i].name);
                ENABLE_VULKAN_DEBUG = false;
            }
        }

    }   
    

    // MARK: CHECK EXTENSION DEPENDENCIES ARE SATISFIED

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);

    if (extensionCount == 0) {
        GLog(WARNING, "No Vulkan extensions supported. Using legacy rendering mode.");
        return false;
    }

    // enumerate extensions
    // ALLOC @extensionProperties
    VkExtensionProperties* extensionProperties = calloc(extensionCount, sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties);

    printf("\tFound extensions:\n");
    for (int i = 0; i < extensionCount; i++) {

        printf("\t\t%s\n", extensionProperties[i].extensionName);

        // set the extension dependency found flag to true
        for (int j = 0; j < numExtensionDependencies; j++) {
            if (strcmp(extensionDependencies[j].name, extensionProperties[i].extensionName) == 0) {
                extensionDependencies[j].supported = true;
            }
        }

    }

    // FREE @extensionProperties
    free(extensionProperties);
    
    // check all extension dependencies are met
    for (int i = 0; i < numExtensionDependencies; i++) { 
        if (!extensionDependencies[i].supported) {
            GLog(WARNING, "Vulkan dependency '%s' not supported. Using legacy rendering mode.", extensionDependencies[i].name);

            return false;
        }
    }

    GLog(INFO, "All Vulkan dependencies satisfied");

    // MARK: CREATE INSTANCE

    // app info
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GSPCore";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "GSPCore";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = numExtensionDependencies;
    createInfo.ppEnabledExtensionNames = dependencyNames;



    if (ENABLE_VULKAN_DEBUG) {
        createInfo.enabledLayerCount = numValidationlayers;
        createInfo.ppEnabledLayerNames = validationLayerNames;
    }

    if (vkCreateInstance(&createInfo, NULL, &vkInstance) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan instance!");
        return false;
    }

    GLog(INFO, "Created Vulkan instance");

    // MARK: CREATE DEBUG CALLBACK
    if (ENABLE_VULKAN_DEBUG) {
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {0};
        debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerCreateInfo.pfnUserCallback = VulkanDebugCallback;
        debugMessengerCreateInfo.pUserData = NULL; // Optional

        // try to create a debug messenger function
        PFN_vkCreateDebugUtilsMessengerEXT createDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT");
    
        if (createDebugMessenger != NULL) {
            if (createDebugMessenger(vkInstance, &debugMessengerCreateInfo, NULL, &vkDebugMessenger) != VK_SUCCESS) {
                GLog(WARNING, "Failed to create Vulkan debug messenger!");
            } else {
                GLog(INFO, "Created Vulkan debug messenger");
            }
        } else {
            GLog(WARNING, "Debug messenger creation not supported. Cannot create debug messenger.");
        }
    }

    // MARK: FIND A PHYSICAL DEVICE
    uint32_t deviceCount = 0;
    if (vkEnumeratePhysicalDevices(vkInstance, &deviceCount, NULL) != VK_SUCCESS || deviceCount == 0) {
        GLog(WARNING, "Failed to get any Vulkan physical devices");
        return false;
    }

    // ALLOC @physicalDevices
    VkPhysicalDevice* physicalDevices = calloc(deviceCount, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, physicalDevices);

    printf("\tFound devices:\n");

    VkPhysicalDevice integratedDevice = NULL;
    VkPhysicalDevice discreteDevice = NULL;

    // iterate over device properties and select suitable devices
    for (int i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
        vkGetPhysicalDeviceFeatures(physicalDevices[i], &deviceFeatures);

        printf("\t\t%s %d\n", deviceProperties.deviceName, deviceProperties.deviceType);

        // assume all devices are suitable
        bool isDeviceSuitable = true;

        uint32_t deviceExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevices[i], NULL, &deviceExtensionCount, NULL);

        if (deviceExtensionCount == 0) {
            isDeviceSuitable = false;
        }

        // check for swapchain support if the device is suitable
        if (isDeviceSuitable) {
            // ALLOC @deviceExtensions
            VkExtensionProperties* deviceExtensions = calloc(deviceExtensionCount, sizeof(VkExtensionProperties));
            vkEnumerateDeviceExtensionProperties(physicalDevices[i], NULL, &deviceExtensionCount, deviceExtensions);

            bool deviceSupportsSwapchain = false;

            //printf("\t\t\t EXTENSIONS:\n");
            for (int i = 0; i < deviceExtensionCount; i++) {
                    //printf("\t\t%s\n", deviceExtensions[i].extensionName);

                if (strcmp(deviceExtensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) { 
                    deviceSupportsSwapchain = true;
                }
            }

            if (!deviceSupportsSwapchain) {
                isDeviceSuitable = false;
            }

            // FREE @deviceExtensions
            free(deviceExtensions);
        }
        

        if (isDeviceSuitable) {
            switch (deviceProperties.deviceType) {
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    integratedDevice = physicalDevices[i];
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    discreteDevice = physicalDevices[i];
                    break;
            }
        }
        
    }

    // FREE @physicalDevices
    free(physicalDevices);

    if (mode == DEFAULT) {
        // prefer integrated graphics
        if (integratedDevice != NULL) {
            vkPhysicalDevice = integratedDevice;
        } else if (discreteDevice != NULL) {
            vkPhysicalDevice = discreteDevice;
        }
    } else if (mode == HIGH_PERFORMANCE_ONLY) {
        // prefer discrete graphics
        if (discreteDevice != NULL) {
            vkPhysicalDevice = discreteDevice;
        } else if (integratedDevice != NULL) {
            vkPhysicalDevice = integratedDevice;
        }
    }

    if (vkPhysicalDevice == NULL) {
        GLog(WARNING, "No suitable Vulkan physical device found. Using Legacy graphics");
        return false;
    }

    // report device info
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);
    GLog(INFO, "Selected GPU device %s", deviceProperties.deviceName);


    return true;

}

// MARK: TRY SETUP WINDOW
bool GHiperRenderManager_TrySetupWindow(GWindow window) {

    GWindowDef* windowDef = (GWindowDef*)window;

    GLog(INFO, "GHiperRenderManager setting up for window");

    VkSurfaceKHR surface;

    #ifdef _WIN32
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {0};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.hwnd = ((GWindowDef*)window)->rawHandle;
        surfaceCreateInfo.hinstance = ((GWindowDef*)window)->hinstance;

        if (vkCreateWin32SurfaceKHR(vkInstance, &surfaceCreateInfo, NULL, &surface) != VK_SUCCESS) {
            GLog(WARNING, "Failed to create Vulkan surface for window!");
            return false;
        }

        GLog(INFO, "Created Vulkan surface for window ok!");
    #endif

    #ifdef __unix__ 
        VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {0};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.window = ((GWindowDef*)window)->rawHandle;
        surfaceCreateInfo.dpy = ((GWindowDef*)window)->display;

        if (vkCreateXlibSurfaceKHR(vkInstance, &surfaceCreateInfo, NULL, &surface) != VK_SUCCESS) {
            GLog(WARNING, "Failed to create Vulkan surface for window!");
            return false;
        }

        GLog(INFO, "Created Vulkan surface for window ok!");
    #endif


    windowDef->vkSurface = surface;

    // MARK: FIND QUEUE FAMILIES
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, NULL);

    // ALLOC @queueFamilyProperties
    VkQueueFamilyProperties* queueFamilyProperties = calloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilyProperties);

    bool foundGraphicsQueueFamily = false;
    bool foundPresentQueueFamily = false;

    for (int i = 0; i < queueFamilyCount; i++) {
        if (queueFamilyProperties[i].queueCount > 0) {
            if (!foundGraphicsQueueFamily && queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                foundGraphicsQueueFamily = true;
                windowDef->graphicsQueueFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, windowDef->vkSurface, &presentSupport);

            if (!foundPresentQueueFamily && presentSupport) {
                foundPresentQueueFamily = true;
                windowDef->presentQueueFamily = i;
            }
        }
    }

    if (!(foundGraphicsQueueFamily && foundPresentQueueFamily)) {
        GLog(WARNING, "No suitable Vulkan queue families found. Using Legacy graphics");
        return false;
    }

    GLog(INFO, "Found suitable queue families");

    // MARK: CREATE LOGICAL DEVICE


    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo[2] = {0};

    queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[0].queueFamilyIndex = windowDef->graphicsQueueFamily;
    queueCreateInfo[0].queueCount = 1;
    queueCreateInfo[0].pQueuePriorities = &queuePriority;

    queueCreateInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[1].queueFamilyIndex = windowDef->presentQueueFamily;
    queueCreateInfo[1].queueCount = 1;
    queueCreateInfo[1].pQueuePriorities = &queuePriority;


    VkDeviceCreateInfo deviceCreateInfo = {0};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
    
    if (windowDef->graphicsQueueFamily == windowDef->presentQueueFamily) {
        deviceCreateInfo.queueCreateInfoCount = 1;
    } else {
        deviceCreateInfo.queueCreateInfoCount = 2;
    }

    // enable GPU features here
    VkPhysicalDeviceFeatures enabledFeatures = {0};

    deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

    // enable swapchain for logical device
    const char* deviceExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    deviceCreateInfo.enabledExtensionCount = 1;
    deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions;

    if (ENABLE_VULKAN_DEBUG) {
        deviceCreateInfo.enabledLayerCount = numValidationlayers;
        deviceCreateInfo.ppEnabledLayerNames = validationLayerNames;
    }

    if (vkCreateDevice(vkPhysicalDevice, &deviceCreateInfo, NULL, &windowDef->vkDevice) != VK_SUCCESS) {
        GLog(WARNING, "Vulkan logical device creation failed. Using Legacy graphics.");
        return false;
    }

    vkGetDeviceQueue(windowDef->vkDevice, windowDef->graphicsQueueFamily, 0, &windowDef->graphicsQueue);
    vkGetDeviceQueue(windowDef->vkDevice, windowDef->presentQueueFamily, 0, &windowDef->presentQueue);

    GLog(INFO, "Created a Vulkan logical device");


    // MARK: CREATE SWAPCHAIN

    // get surface formats and present modes
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice,  windowDef->vkSurface, &formatCount, NULL);

    // ALLOC @surfaceFormats
    VkSurfaceFormatKHR* surfaceFormats = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, windowDef->vkSurface, &formatCount, surfaceFormats);

    VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];
    
    for (int i = 0; i < formatCount; i++) {
        if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {
            surfaceFormat = surfaceFormats[i];
        }
    }

    // FREE @surfaceFormats
    free(surfaceFormats);

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice,  windowDef->vkSurface, &presentModeCount, NULL);

    // ALLOC @presentModes
    VkPresentModeKHR* presentModes = calloc(presentModeCount, sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice,  windowDef->vkSurface,  &presentModeCount, presentModes);
    
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    
    for (int i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = presentModes[i];
        }
    }

    // FREE @presentModes
    free(presentModes);

    VkExtent2D extent;

    VkSurfaceCapabilitiesKHR capabilities = {0};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, windowDef->vkSurface, &capabilities);

    if (capabilities.currentExtent.width != UINT32_MAX) {
        extent = capabilities.currentExtent;
    } else if (capabilities.minImageExtent.width < windowDef->width && 
               capabilities.maxImageExtent.width > windowDef->width && 
               capabilities.minImageExtent.height < windowDef->height && 
               capabilities.maxImageExtent.height > windowDef->height) {
        extent = (VkExtent2D) { windowDef->width, windowDef->height };
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {0};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = windowDef->vkSurface;

    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = { windowDef->graphicsQueueFamily, windowDef->presentQueueFamily};

    if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapchainCreateInfo.preTransform = capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;

    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(windowDef->vkDevice, &swapchainCreateInfo, NULL, &windowDef->vkSwapchain) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan swapchain!");
        return false;
    }
  
    windowDef->vkSwapchainImageFormat = surfaceFormat.format;
    windowDef->vkSwapchainExtent = extent;


    // MARK: CREATE IMAGES

    vkGetSwapchainImagesKHR(windowDef->vkDevice, windowDef->vkSwapchain, &windowDef->vkImageCount, NULL);

    // ALLOC @windowDef->vkImages
    windowDef->vkImages = calloc(windowDef->vkImageCount, sizeof(VkImage));
    vkGetSwapchainImagesKHR(windowDef->vkDevice, windowDef->vkSwapchain, &windowDef->vkImageCount, windowDef->vkImages);
    
    // ALLOC @windowDef->vkImageViews
    windowDef->vkImageViews = calloc(windowDef->vkImageCount, sizeof(VkImageView));

    VkImageViewCreateInfo imageViewCreateInfo = {0};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = windowDef->vkSwapchainImageFormat;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;

    for (int i = 0; i < windowDef->vkImageCount; i++) {
        imageViewCreateInfo.image = windowDef->vkImages[i];
        if (vkCreateImageView(windowDef->vkDevice, &imageViewCreateInfo, NULL, &windowDef->vkImageViews[i]) != VK_SUCCESS) {
            GLog(WARNING, "Failed to create Vulkan image view!");
            return false;
        }
    }

    // MARK : CREATE RENDER PASS

    VkAttachmentDescription colorAttachment = {0};
    colorAttachment.format = windowDef->vkSwapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {0};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {0};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {0};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {0};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(windowDef->vkDevice, &renderPassInfo, NULL, &windowDef->vkRenderPass) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan renderpass!");
        return false;
    }


    // MARK: CREATE PIPELINE


    // load compiled shaders
    char* vertShaderCode;
    long vertShaderSize;
    char* fragShaderCode;
    long fragShaderSize;

    FILE *fileptr;

    fileptr = fopen("shaders/vert.spv", "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    vertShaderSize = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    vertShaderCode = (char *)malloc(vertShaderSize * sizeof(char)); // Enough memory for the file
    fread(vertShaderCode, vertShaderSize, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    fileptr = fopen("shaders/frag.spv", "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    fragShaderSize = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    fragShaderCode = (char *)malloc(fragShaderSize * sizeof(char)); // Enough memory for the file
    fread(fragShaderCode, fragShaderSize, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file


    // create vertex shader
    VkShaderModuleCreateInfo vertexShaderCreateInfo = {0};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderCreateInfo.codeSize = vertShaderSize;
    vertexShaderCreateInfo.pCode = (uint32_t*)(vertShaderCode);

    VkShaderModule vertShaderModule;
    if (vkCreateShaderModule(windowDef->vkDevice, &vertexShaderCreateInfo, NULL, &vertShaderModule) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan vertex shader");
        return false;
    }

    // create fragment shader
    VkShaderModuleCreateInfo fragmentShaderCreateInfo = {0};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderCreateInfo.codeSize = fragShaderSize;
    fragmentShaderCreateInfo.pCode = (uint32_t*)(fragShaderCode);

    VkShaderModule fragShaderModule;
    if (vkCreateShaderModule(windowDef->vkDevice, &fragmentShaderCreateInfo, NULL, &fragShaderModule) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan fragment shader");
        return false;
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {0};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {0};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {0};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {0};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {0};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {0};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {0};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {0};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {0};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState = {0};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {0};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(windowDef->vkDevice, &pipelineLayoutInfo, NULL, &windowDef->vkPipelineLayout) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan pipeline layout");
        return false;
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {0};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = windowDef->vkPipelineLayout;
    pipelineInfo.renderPass = windowDef->vkRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(windowDef->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &windowDef->vkPipeline) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan pipeline");
        return false;
    }

    vkDestroyShaderModule(windowDef->vkDevice, fragShaderModule, NULL);
    vkDestroyShaderModule(windowDef->vkDevice, vertShaderModule, NULL);

    printf("SO FAR SO GOOD\n");

    // MARK: CREATE FRAMEBUFFERS

    // ALLOC @windowDef->vkFramebuffers
    windowDef->vkFramebuffers = calloc(windowDef->vkImageCount, sizeof(VkFramebuffer));

    for (int i = 0; i < windowDef->vkImageCount; i++) {

        VkImageView attachments[] = {
            windowDef->vkImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {0};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = windowDef->vkRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = windowDef->vkSwapchainExtent.width;
        framebufferInfo.height = windowDef->vkSwapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(windowDef->vkDevice, &framebufferInfo, NULL, &windowDef->vkFramebuffers[i]) != VK_SUCCESS) {
            GLog(WARNING, "Failed to create Vulkan framebuffer");
            return false;
        }
    }


    // CREATE COMMAND POOL

    VkCommandPoolCreateInfo poolInfo = {0};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = windowDef->graphicsQueueFamily;

    if (vkCreateCommandPool(windowDef->vkDevice, &poolInfo, NULL, &windowDef->vkCommandPool) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan command pool");
        return false;
    }

    // CREATE COMMAND BUFFER

    VkCommandBufferAllocateInfo allocInfo = {0};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = windowDef->vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(windowDef->vkDevice, &allocInfo, &windowDef->vkCommandBuffer) != VK_SUCCESS) {
        GLog(WARNING, "Failed to create Vulkan command buffer");
        return false;
    }


    // SET UP SEMAPHORES
    VkSemaphoreCreateInfo semaphoreCreateInfo = {0};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(windowDef->vkDevice, &semaphoreCreateInfo, NULL, &windowDef->imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(windowDef->vkDevice, &semaphoreCreateInfo, NULL, &windowDef->renderingFinishedSemaphore) != VK_SUCCESS) {
        GLog(WARNING, "Vulkan semaphore creation failed.");
        return false;
    }

    VkFenceCreateInfo fenceInfo = {0};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(windowDef->vkDevice, &fenceInfo, NULL, &windowDef->inFlightFence) != VK_SUCCESS) {

        GLog(WARNING, "Vulkan semaphore creation failed.");
        return false;
    }



    return true;
}   


// MARK: RENDER FRAME
void GHiperRenderManager_RenderWindow(GWindow window) {

    GWindowDef* windowDef = (GWindowDef*)window;

    GLog(INFO, "High Performance Render Manager rendering...");

    vkWaitForFences(windowDef->vkDevice, 1, &windowDef->inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(windowDef->vkDevice, 1, &windowDef->inFlightFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(windowDef->vkDevice, windowDef->vkSwapchain, UINT64_MAX, windowDef->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    vkResetCommandBuffer(windowDef->vkCommandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
    
    // MARK: RECORD COMMAND BUFFER

    VkCommandBufferBeginInfo beginInfo = {0};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(windowDef->vkCommandBuffer, &beginInfo) != VK_SUCCESS) {
        GLog(INFO, "Failed to begin recording Vulkan command buffer!");
        return;
    }

    VkRenderPassBeginInfo renderPassInfo = {0};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = windowDef->vkRenderPass;
    renderPassInfo.framebuffer = windowDef->vkFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = (VkOffset2D){0, 0};
    renderPassInfo.renderArea.extent = windowDef->vkSwapchainExtent;

    VkClearValue clearColor = {{{1.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(windowDef->vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(windowDef->vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, windowDef->vkPipeline);

    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) windowDef->vkSwapchainExtent.width;
    viewport.height = (float) windowDef->vkSwapchainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(windowDef->vkCommandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {0};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent =  windowDef->vkSwapchainExtent;
    vkCmdSetScissor(windowDef->vkCommandBuffer, 0, 1, &scissor);

    vkCmdDraw(windowDef->vkCommandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(windowDef->vkCommandBuffer);

    if (vkEndCommandBuffer(windowDef->vkCommandBuffer) != VK_SUCCESS) {
        GLog(INFO, "Failed to record Vulkan command buffer!");
        return;
    }

    VkSubmitInfo submitInfo = {0};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {windowDef->imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &windowDef->vkCommandBuffer;

    VkSemaphore signalSemaphores[] = {windowDef->imageAvailableSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(windowDef->graphicsQueue, 1, &submitInfo, windowDef->inFlightFence) != VK_SUCCESS) {
        GLog(INFO, "Failed to submit draw Vulkan command buffer!");
        return;
    }

    VkPresentInfoKHR presentInfo = {0};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {windowDef->vkSwapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(windowDef->presentQueue, &presentInfo);
}

void GHiperRenderManager_CleanupWindow(GWindow window) {

    vkDestroySurfaceKHR(vkInstance, ((GWindowDef*)window)->vkSurface, NULL);
    

}

void GHiperRenderManager_Cleanup() {

    // DESTROY DEBUG CALLBACK
    if (ENABLE_VULKAN_DEBUG) { 
        PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");

        if (destroyDebugMessenger != NULL) {
            destroyDebugMessenger(vkInstance, vkDebugMessenger, NULL);
            GLog(INFO, "Destroyed Vulkan debug messenger");
        } else {
            GLog(WARNING, "Debug messenger destroying not supported. Cannot destroy debug messenger.");
        }

    }


    
    vkDestroyInstance(vkInstance, NULL);


}