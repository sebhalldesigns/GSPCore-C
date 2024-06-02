#ifndef GRENDERER_H
#define GRENDERER_H

// GRenderer.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GRenderer is a generic 2D renderer object
// that abstracts various underlying technologies
// to provide a simple, singular API for rendering.
// It is provided on a per-window basis.

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"

#include <stdbool.h>
#include <stddef.h>

// a window 
GRenderer GRenderer_Init();

void GRenderer_Free(GRenderer renderer);

// GRenderer_CreatePipeline etc

void GRenderer_RenderSelf(GRenderer renderer);
void GRenderer_RenderRoot(GRenderer renderer);

typedef enum {
    GRAPHICS_TYPE_NONE,
    GRAPHICS_TYPE_WEBGL,
    GRAPHICS_TYPE_METAL,
    GRAPHICS_TYPE_VULKAN
} GRendererGraphicsType;

typedef struct {
    GRendererGraphicsType graphicsType;
    char* graphicsDevice;
    size_t imagesPerDrawCall;
} GRendererSystemInfo;

#ifdef GSPCORE_BUILD_WASM

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

GLuint WebGLCompileShader(GLenum type, const char* source);
GLuint WebGLCreateProgram(GLuint vertexShader, GLuint fragmentShader);

typedef struct {
    GLuint framebuffer;
    GLuint renderedTexture;
    GSize textureSize;
    GView view;
} GRendererDef;

#endif

#ifdef GSPCORE_BUILD_MACOS

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <CoreText/CoreText.h>

typedef struct {
    id<MTLDevice> mtlDevice;
    id<MTLRenderPipelineState> mtlPipelineState;
    id<MTLBuffer> mtlVertexBuffer;
    id<MTLTexture> mtlTexture;
} GRendererDef;

#endif



#endif // GRENDERER_H
