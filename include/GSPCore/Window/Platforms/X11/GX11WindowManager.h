#ifndef GX11WINDOWMANAGER_H
#define GX11WINDOWMANAGER_H

#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
    Display* display;
    Window root;
    XEvent event;
    KeySym eventKey;
    char eventText[256];
} GX11State;

bool GX11WindowManager_TryInit();
GWindow* GX11WindowManager_OpenWindow();
int GX11WindowManager_RunLoop(GApplication* app);

#endif // GX11WINDOWMANAGER_H