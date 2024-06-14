#include "GSPCore/GSPCore.h"
#include "GSPCore/Windowing/Platforms/Wayland/GWaylandWindowManager.h"
#include "GSPCore/Windowing/Platforms/X11/GX11WindowManager.h"

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

GWindow GWindowManager_OpenWindow() {

    switch (windowingSystem) {
        case WINDOWING_SYSTEM_WAYLAND:
            return GWaylandWindowManager_OpenWindow();

        default:
            GWindow fake;
            return fake;
    }
}
