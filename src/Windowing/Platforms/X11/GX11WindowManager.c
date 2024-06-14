#include <GSPCore/GSPCore.h>
#include <GSPCore/Windowing/Platforms/X11/GX11WindowManager.h>

// X11 headers
#include <X11/Xlib.h>

static Display* display = NULL;

bool GX11WindowManager_TryInit() {

    display = XOpenDisplay(NULL);

    if (display == NULL) {
        GLog(FAIL, "No X11 display server found!");
        return false;
    }


    GLog(INFO, "Started WindowManager on X11 backend");

    return true;
}   
