#ifndef GWINDOWMANAGER_H
#define GWINDOWMANAGER_H

#include <stdbool.h>
#include "GWindow.h"

typedef enum {
    WINDOWING_SYSTEM_NONE,
    WINDOWING_SYSTEM_WAYLAND,
    WINDOWING_SYSTEM_X11,
    WINDOWING_SYSTEM_COCOA,
    WINDOWING_SYSTEM_WIN32,
    WINDOWING_SYSTEM_IOS,
    WINDOWING_SYSTEM_ANDROID,
    WINDOWING_SYSTEM_WEB
} GWindowManagerWindowingSystem;

bool GWindowManager_Init();

GWindow* GWindowManager_OpenWindow();

GWindowManagerWindowingSystem GWindowManager_GetWindowingSystem();

int GWindowManager_RunLoop();

#endif // GWINDOWMANAGER_H