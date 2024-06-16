#ifndef GVULKANRENDERMANAGER_H
#define GVULKANRENDERMANAGER_H

// GVulkanRenderManager.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GVulkanRenderManager is a vulkan-specific renderer implementation.

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>

bool GVulkanRenderManager_Init();
bool GVulkanRenderManager_SetupWindow(GWindow* window);
void GVulkanRenderManager_RenderWindow(GWindow* window);

#endif // GVULKANRENDERMANAGER_H







