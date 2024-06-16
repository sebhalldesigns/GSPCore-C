#ifndef GVULKANRENDERMANAGER_H
#define GVULKANRENDERMANAGER_H

// GVulkanRenderManager.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GVulkanRenderManager is a vulkan-specific renderer implementation.

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>

bool GVulkanRenderManager_Init();
bool GVulkanRenderManager_SetupWindow(GWindow* window);
void GVulkanRenderManager_RenderWindow(GWindow* window);

uint32_t GVulkanRenderManager_FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

void GVulkanRenderManager_CreateVertexBuffer(VkBuffer* buffer, VkDeviceMemory* bufferMemory);

void GVulkanRenderManager_CreateUniformBuffer(size_t size, VkBuffer* buffer, VkDeviceMemory* bufferMemory);


#endif // GVULKANRENDERMANAGER_H







