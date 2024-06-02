#ifndef GWINDOW_H
#define GWINDOW_H

// GWindow.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GWindow is a generic top-level application window

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"

typedef struct {
    const char* title;
    GSize size;
} GWindowInfo;

GWindow GWindow_Init(GWindowInfo info);
void GWindow_Free(GWindow window);


void GWindow_SetController(GWindow window, GWindowController windowController);


void GWindow_Close(GWindow window);
// Request a window render. This should be handled automatically by the system when a window is resized etc.
void GWindow_Render(GWindow window);
void GWindow_Layout(GWindow window);

GView GWindow_GetRootView(GWindow window);
void GWindow_SetRootView(GWindow window, GView view);

#ifdef GSPCORE_BUILD_WASM

typedef struct {
    GWindowInfo info;
    GWindowController controller;
    GView rootView;
} GWindowDef;

#endif



#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface CocoaWindow : NSWindow
@property GWindow window;
@end

@interface CocoaMetalView: MTKView
@property GWindow mywindow;
@end

typedef struct {
    CocoaWindow* cocoaWindow;
    CocoaMetalView* cocoaMetalView;
    GWindowInfo info;
    GRenderer renderer;
    GWindowController controller;
} GWindowDef;

#endif

#ifdef GSPCORE_BUILD_WIN32

#include <Windows.h>

typedef struct {
    HWND rawHandle;
    GWindowInfo info;
    GRenderer renderer;
    GWindowController controller;
} GWindowDef;

#endif




#endif // GWINDOW_H