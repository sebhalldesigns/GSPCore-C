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
            GWindow* window = GWaylandWindowManager_OpenWindow();

            if (GRenderManager_SetupWindow(window)) {
                return window;
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
        default:
            return -1;
    }   
}

GWindowManagerWindowingSystem GWindowManager_GetWindowingSystem() {
    return windowingSystem;
}

