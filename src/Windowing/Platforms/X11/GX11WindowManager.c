#include <GSPCore/GSPCore.h>
#include <GSPCore/Windowing/Platforms/X11/GX11WindowManager.h>
#include <GSPCore/Graphics/GRenderManager.h>

// X11 headers
#include <X11/Xlib.h>

static GX11State state;
static GWindow* rootWindow = NULL;


bool GX11WindowManager_TryInit() {

    state.display = XOpenDisplay(NULL);
    state.root = DefaultRootWindow(state.display);

    if (state.display == NULL || state.root == 0) {
        GLog(FAIL, "Failed to connect to X11 server!");
        return false;
    }

    GLog(INFO, "Started GWindowManager on X11 backend");

    return true;
}   


GWindow* GX11WindowManager_OpenWindow() {

    GWindow* window = calloc(1, sizeof(GWindow));

    window->platformHandles.xDisplay = state.display;
    window->platformHandles.xWindow = XCreateSimpleWindow(state.display, state.root, 0, 0, 100, 100, 0, 0, 0);

    if (window->platformHandles.xWindow == None) {
        DEBUG_LOG(FAIL, "Failed to create an X11 window");
        free(window);
        return NULL;
    }

    XStoreName(state.display, window->platformHandles.xWindow, "example X11 window");

    Atom deleteMessage = XInternAtom(state.display, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(state.display, window->platformHandles.xWindow, &deleteMessage, 1);
    XSelectInput(state.display, window->platformHandles.xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(state.display, window->platformHandles.xWindow);

    window->platformHandles.xDeleteAtom = deleteMessage;

    rootWindow = window;
    return window;
}

int GX11WindowManager_RunLoop() {

    while (true) {
        if (!XPending(state.display)) {  
            // possibly redraw here
           
        }

        XNextEvent(state.display, &state.event);

        switch(state.event.type) {
            case ConfigureNotify:
                
                printf("CONFIGURE NOTIFY\n");
                
                int newWidth = state.event.xconfigure.width;
                int newHeight = state.event.xconfigure.height;


                
                if (rootWindow != NULL) {
                    GRenderManager_RenderWindow(rootWindow);
                    rootWindow->frame.size.width = newWidth;
                    rootWindow->frame.size.height = newHeight;
                }


                break;


            case MotionNotify:

                printf("MOTION NOTIFY\n");


                break;
            case ButtonPress:
            
                printf("BUTTON PRESS\n");

                break;
            case ButtonRelease:

                printf("BUTTON RELEASE\n");

                break;
            case KeyPress:
                // Handle keyboard events
                XLookupString(&state.event.xkey, state.eventText, sizeof(state.eventText), &state.eventKey, 0);
                printf("Key pressed: %s\n", state.eventText);
                break;

            case ClientMessage:
                
                //if (state.Event.xclient.data.l[0] == (Atom)windowDef->xDeleteAtom) {

                    printf("CLIENT MESSAGE\n");
                //}

                


                break;
        }

    }
}