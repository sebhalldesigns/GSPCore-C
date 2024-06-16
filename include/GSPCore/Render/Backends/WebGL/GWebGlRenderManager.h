#ifndef GWEBGLRENDERMANAGER_H
#define GWEBGLRENDERMANAGER_H

// GVulkanRenderManager.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GVulkanRenderManager is a vulkan-specific renderer implementation.

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <stdbool.h>

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

bool GWebGlRenderManager_Init();
bool GWebGlRenderManager_SetupWindow(GWindow* window);
void GWebGlRenderManager_RenderWindow(GWindow* window);

#endif // GWEBGLRENDERMANAGER_H


