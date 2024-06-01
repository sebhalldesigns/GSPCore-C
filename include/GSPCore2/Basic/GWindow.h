#ifndef GWINDOW_H
#define GWINDOW_H

// GWindow.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GWindow is a generic top-level application window

#include "../GSPCoreGeometry.h"
#include "../Controllers/GWindowController.h"

// MARK: DEFINITION
typedef void* GWindow;
typedef void* GWindowController;

typedef struct {
    const char* title;
    GSize size;
} GWindowInfo;

GWindow GWindow_Init(GWindowInfo info);
void GWindow_Free(GWindow window);


void GWindow_SetController(GWindow window, GWindowController windowController);

// Request a window render. This should be handled automatically by the system when a window is resized etc.
void GWindow_Render(GWindow window);

#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

@interface CocoaWindow : NSWindow
@property GWindow window;
@end

typedef struct {
    CocoaWindow* cocoaWindow;
    GWindowInfo info;
    GWindowController controller;
} GWindowDef;

#endif




#endif // GWINDOW_H