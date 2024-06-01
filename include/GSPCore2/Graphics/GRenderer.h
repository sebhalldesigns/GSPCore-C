#ifndef GRENDERER_H
#define GRENDERER_H

// GRenderer.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GRenderer is a generic 2D renderer object
// that abstracts various underlying technologies
// to provide a simple, singular API for rendering.
// It is provided on a per-window basis.

#include "../Basic/GWindow.h"
#include "../GSPCoreGeometry.h"

#include <stdbool.h>

typedef void* GWindow;
typedef void* GRenderer;

GRenderer GRenderer_InitForWindow(GWindow window);
void GRenderer_Free(GRenderer renderer);


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
