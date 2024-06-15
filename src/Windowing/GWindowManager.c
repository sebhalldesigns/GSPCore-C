#include "GSPCore/GSPCore.h"

#ifdef GSPCORE_BUILD_UNIX
#include "GSPCore/Windowing/Platforms/Wayland/GWaylandWindowManager.h"
#include "GSPCore/Windowing/Platforms/X11/GX11WindowManager.h"
#elif GSPCORE_BUILD_WIN32
#include "GSPCore/Windowing/Platforms/Win32/GWin32WindowManager.h"
#elif GSPCORE_BUILD_WEB
#include "GSPCore/Windowing/Platforms/Web/GWebWindowManager.h"
#endif

#include <GSPCore/Graphics/GRenderManager.h>

static GWindowManagerWindowingSystem windowingSystem = WINDOWING_SYSTEM_NONE;

bool GWindowManager_Init() {
    GApplicationEnvironment environment = GApplication_GetEnvironment();

    switch (environment) {

    #ifdef GSPCORE_BUILD_UNIX
        case ENVIRONMENT_UNIX:
            if (GWaylandWindowManager_TryInit()) {
                windowingSystem = WINDOWING_SYSTEM_WAYLAND;
                return true;
            }

            if (GX11WindowManager_TryInit()) {
                windowingSystem = WINDOWING_SYSTEM_X11;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WIN32
        case ENVIRONMENT_WIN32:
            if (GWin32WindowManager_TryInit()) {
                windowingSystem = WINDOWING_SYSTEM_WIN32;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WEB
        case ENVIRONMENT_WEB:
            if (GWebWindowManager_TryInit()) {
                windowingSystem = WINDOWING_SYSTEM_WEB;
                return true;
            }

            return false;
    #endif
        default:
            return false;
    }

}

GWindow* GWindowManager_OpenWindow() {

    switch (windowingSystem) {

    #ifdef GSPCORE_BUILD_UNIX

        case WINDOWING_SYSTEM_WAYLAND:
            GWindow* waylandWindow = GWaylandWindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(waylandWindow)) {
                return waylandWindow;
            }
            break;

        case WINDOWING_SYSTEM_X11:
            GWindow* x11Window = GX11WindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(x11Window)) {
                return x11Window;
            }

            break;
    #elif GSPCORE_BUILD_WIN32
        case WINDOWING_SYSTEM_WIN32:
            GWindow* win32Window = GWin32WindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(win32Window)) {
                return win32Window;
            }

            break;
    #elif GSPCORE_BUILD_WEB
        case WINDOWING_SYSTEM_WEB:
            GWindow* webWindow = GWebWindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(webWindow)) {
                printf("RENDER MANAGER DONE\n");
                return webWindow;
            }

            break;
    #endif
        default:
            break;
    }

    return NULL;
}

int GWindowManager_RunLoop() {

    switch (windowingSystem) {
    #ifdef GSPCORE_BUILD_UNIX

        case WINDOWING_SYSTEM_WAYLAND:
            return GWaylandWindowManager_RunLoop();
        case WINDOWING_SYSTEM_X11:
            return GX11WindowManager_RunLoop();
    #elif GSPCORE_BUILD_WIN32
        case WINDOWING_SYSTEM_WIN32:
            return GWin32WindowManager_RunLoop();
    #elif GSPCORE_BUILD_WEB
        case WINDOWING_SYSTEM_WEB:
            return GWebWindowManager_RunLoop();
    #endif
        default:
            return -1;
    }   
}

GWindowManagerWindowingSystem GWindowManager_GetWindowingSystem() {
    return windowingSystem;
}

