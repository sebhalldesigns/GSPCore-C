#ifndef GWINDOWCONTROLLER_H
#define GWINDOWCONTROLLER_H

// GWindowController.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GWindowController is an object that
// recieves and handles events for instances
// of GWindow.

#include "../GSPCoreGeometry.h"
#include "../Basic/GWindow.h"

#include <stdbool.h>

typedef void* GWindow;

typedef void* GWindowController;

typedef void(*GWindowCloseEvent)(GWindow window);
typedef bool(*GWindowWillCloseEvent)(GWindow window);
typedef void (*GWindowResizeEvent)(GWindow window, GSize newSize);


GWindowController GWindowController_Init();
void GWindowController_Free(GWindowController windowController);

void GWindowController_SetResizeEvent(GWindowController windowController, GWindowResizeEvent resizeEvent);


#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

@interface CocoaWindowDelegate : NSObject <NSWindowDelegate>
@property GWindowController controller;
@end

typedef struct {
    CocoaWindowDelegate* cocoaWindowDelegate;
    GWindowResizeEvent resizeEvent;
} GWindowControllerDef;

#endif

#ifdef GSPCORE_BUILD_WIN32

typedef struct {
    GWindowResizeEvent resizeEvent;
} GWindowControllerDef;

#endif


#endif // GWINDOWCONTROLLER_H