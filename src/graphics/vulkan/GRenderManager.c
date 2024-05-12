// https://github.com/seb-hall/UniversalUI/blob/5bcbce94ba820519da9586a627d0409d2ebd115a/Rendering/source/uRenderManager.cpp

#include "GSPCore/GRenderManager.h"
#include "GSPCore/GLog.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <dlfcn.h>

#include <vulkan/vulkan.h>

void* vulkanLibrary = NULL;

#ifdef DEBUG
    bool ENABLE_VULKAN_DEBUG = true;
#else
    bool ENABLE_VULKAN_DEBUG = false;
#endif

VkInstance vkInstance = NULL;
VkDebugUtilsMessengerEXT vkDebugMessenger = NULL;
VkPhysicalDevice vkPhysicalDevice = NULL;
VkDevice vkDevice;
uint32_t graphicsQueueFamily = 0;
uint32_t presentQueueFamily = 0;


typedef struct {
    const char* name;
    bool supported;
} VulkanDependency;

VulkanDependency validationLayers[] = {
    (VulkanDependency) { "VK_LAYER_KHRONOS_validation", false },
};

const char* validationLayerNames[] = {
    "VK_LAYER_KHRONOS_validation",
};

VulkanDependency extensionDependencies[] = {
    (VulkanDependency) { "VK_KHR_surface", false },
    #ifdef _WIN32
    (VulkanDependency) { "VK_KHR_win32_surface", false },
    #endif

    #ifdef __unix__
    (VulkanDependency) { "VK_KHR_wayland_surface", false },
    (VulkanDependency) { "VK_KHR_xlib_surface", false },
    #endif

    #ifdef DEBUG
    (VulkanDependency) { "VK_EXT_debug_report", false },
    (VulkanDependency) { "VK_EXT_debug_utils", false },
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



void GRenderManager_Init(GRenderManagerMode mode) { 

}

bool GRenderManager_TryInitHighPerformance(GRenderManagerMode mode) {
    GLog(INFO, "Starting Render Manager...");

    // try to load the vulkan library
    #if WIN32
        vulkanLibrary = LoadLibrary( "vulkan-1.dll" );
    #elif __unix__
        vulkanLibrary = dlopen( "libvulkan.so",  RTLD_NOW | RTLD_GLOBAL );
    #endif

    if (vulkanLibrary == NULL) {
        GLog(WARNING, "Vulkan library not found. Using legacy rendering mode.");
        return false;
    } 
    
    GLog(INFO, "Vulkan library loaded");

    // CHECK FOR VALIDATION LAYERS
    
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
    

    // CHECK EXTENSION DEPENDENCIES ARE SATISFIED

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

    // CREATE INSTANCE

    // app info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GSPCore";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "GSPCore";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
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

    // CREATE DEBUG CALLBACK
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

    // FIND A PHYSICAL DEVICE
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

    
    // FIND QUEUE FAMILIES
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
                graphicsQueueFamily = i;
            }

            if (!foundPresentQueueFamily && queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                foundPresentQueueFamily = true;
                presentQueueFamily = i;
            }
        }
    }

    if (!(foundGraphicsQueueFamily && foundPresentQueueFamily)) {
        GLog(WARNING, "No suitable Vulkan queue families found. Using Legacy graphics");
        return false;
    }

    GLog(INFO, "Found suitable queue families");

    // CREATE LOGICAL DEVICE


    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo[2] = {0};

    queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[0].queueFamilyIndex = graphicsQueueFamily;
    queueCreateInfo[0].queueCount = 1;
    queueCreateInfo[0].pQueuePriorities = &queuePriority;

    queueCreateInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[1].queueFamilyIndex = presentQueueFamily;
    queueCreateInfo[1].queueCount = 1;
    queueCreateInfo[1].pQueuePriorities = &queuePriority;


    VkDeviceCreateInfo deviceCreateInfo = {0};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
    
    if (graphicsQueueFamily == presentQueueFamily) {
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

    if (vkCreateDevice(vkPhysicalDevice, &deviceCreateInfo, NULL, &vkDevice) != VK_SUCCESS) {
        GLog(WARNING, "Vulkan logical device creation failed. Using Legacy graphics.");
        return false;
    }

    GLog(INFO, "Created a Vulkan logical device");


}



void GRenderManager_Cleanup() {

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


}