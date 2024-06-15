#include "GSPCore/GSPCore.h"
#include "GSPCore/Windowing/Platforms/Wayland/GWaylandWindowManager.h"
#include "GSPCore/Windowing/Platforms/X11/GX11WindowManager.h"

#include <GSPCore/Graphics/GRenderManager.h>

static GWindowManagerWindowingSystem windowingSystem = WINDOWING_SYSTEM_NONE;

bool GWindowManager_Init() {
    GApplicationEnvironment environment = GApplication_GetEnvironment();

    switch (environment) {
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
        default:
            return false;
    }

}

GWindow* GWindowManager_OpenWindow() {

    switch (windowingSystem) {
        case WINDOWING_SYSTEM_WAYLAND:
            GWindow* waylandWindow = GWaylandWindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(waylandWindow)) {
                return waylandWindow;
            }

        case WINDOWING_SYSTEM_X11:
            GWindow* x11Window = GX11WindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(x11Window)) {
                return x11Window;
            }

            break;

        default:
            break;
    }

    return NULL;
}

int GWindowManager_RunLoop() {

    switch (windowingSystem) {
        case WINDOWING_SYSTEM_WAYLAND:
            return GWaylandWindowManager_RunLoop();
        case WINDOWING_SYSTEM_X11:
            return GX11WindowManager_RunLoop();
        default:
            return -1;
    }   
}

GWindowManagerWindowingSystem GWindowManager_GetWindowingSystem() {
    return windowingSystem;
}

