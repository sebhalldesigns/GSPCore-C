#ifndef GMETALRENDERMANAGER_H
#define GMETALRENDERMANAGER_H

// GMetalenderManager.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GMetalenderManager is a vulkan-specific renderer implementation.

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>


bool GMetalRenderManager_Init();
bool GMetalRenderManager_SetupWindow(GWindow* window);
void GMetalRenderManager_RenderWindow(GWindow* window);

char* GMetalRenderManager_LoadShader(char* path);

#endif // GMETALRENDERMANAGER_H


