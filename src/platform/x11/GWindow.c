#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"

#include <math.h>

#include "GSPCore/Graphics/GGI.h"

#include "internal/def/Graphics/GFrameDef.h"
#include "internal/def/Graphics/GShaderDef.h"
#include "internal/def/Graphics/GTextureDef.h"
#include "internal/def/Graphics/GVertexBufferDef.h"
#include "GSPCore/GVector.h"

#include "GSPCore/GRenderManager.h"

#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <time.h>


// common statics
static GVector windowVector = NULL;

static Display* xDisplay = NULL;
static Window xRoot = None;
static XEvent xEvent;
static KeySym eventKey;
static char eventText[255];

//static Atom deleteMessage;


clock_t start = 0;
clock_t end = 0;
double cpu_time_used;

clock_t lastFrameTime = 0;

void RenderWindow(GWindow window);

bool ctxErrorOccurred = false;
int ctxErrorHandler( Display *dpy, XErrorEvent *ev) {
    ctxErrorOccurred = true;
    return 0;
}



// functions defined in this file
GWindowDef* TryGetWindow(Window xWindow);

bool GWindowDef_Init() {
    return true;
}

GWindow GWindow_Init(GWindowInfo info) {

    if (xDisplay == NULL) {
        // either first time opening a window, or x11 broken.
        xDisplay = XOpenDisplay(NULL);
        xRoot = DefaultRootWindow(xDisplay);
    }

    if (xDisplay == NULL) {
        DEBUG_LOG(FAIL, "Failed to connect to an X11 display server!");
        return NULL;
    }

    if (xRoot == 0) {
        XCloseDisplay(xDisplay);
        DEBUG_LOG(FAIL, "Failed to connect to an X11 root window!");
        return NULL;
    }



    Window xWindow = XCreateSimpleWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 0, 0);

    if (xWindow == None) {
        DEBUG_LOG(FAIL, "Failed to create an X11 window");
        return 0;
    }

    XStoreName(xDisplay, xWindow, info.title);

    Atom deleteMessage = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(xDisplay, xWindow, &deleteMessage, 1);
    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(xDisplay, xWindow);



    GWindowDef* window =  calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    

    window->xDeleteAtom = (uintptr_t)deleteMessage;
    window->title = info.title;
    window->rawHandle = (void*)xWindow;
    window->display = (void*)xDisplay;
    window->width = info.width;
    window->height = info.height;

    GRenderManager_SetupWindow(window);
    

    if (windowVector == NULL) {
        windowVector = GVector_Init();
    }

    if (windowVector != NULL) {
        GVector_Add(windowVector, (GVectorItem)window);
        DEBUG_LOG(INFO, "Allocated GWindow at %lu", window);
        return window;
    } else {
        DEBUG_LOG(FAIL, "Failed to allocate a window vector.");
        return NULL;
    }

}

void GWindow_SetUserData(GWindow window, void* userData) {
    if (window == NULL) {
        return NULL;
    }

    ((GWindowDef*)window)->userData = userData;

}

void GWindow_SetDrawDelegate(GWindow window, GWindowDrawDelegate drawDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->drawDelegate = drawDelegate;
    }
}

void GWindow_SetCloseDelegate(GWindow window, GWindowCloseDelegate closeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->closeDelegate = closeDelegate;
    }
}
void GWindow_SetWillCloseDelegate(GWindow window, GWindowWillCloseDelegate willCloseDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->willCloseDelegate = willCloseDelegate;
    }
}

void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->resizeDelegate = resizeDelegate;
    }
}

void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->pointerMoveDelegate = pointerMoveDelegate;
    }
}

void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonDownDelegate = buttonDownDelegate;
    }
}

void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonUpDelegate = buttonUpDelegate;
    }
}

void GWindow_Close(GWindow window) {
    if (GVector_Contains(windowVector, window)) {
        
        GRenderManager_CleanupWindow(window);

        GVector_Remove(windowVector, (GVectorItem)window);

        DEBUG_LOG(INFO, "Closed GWindow at %lu", (uintptr_t)window);

    }
}

void GWindow_Redraw(GWindow window) {
    if (GVector_Contains(windowVector, window)) {

        GWindowDef* windowDef = (GWindowDef*)window;
    
        windowDef->redrawFlag = true;
    }
}



void GWindowDef_Poll() {



    if (!XPending(xDisplay)) { 
        // /printf("NO EVENTS\n");
        
        for (int i = 0; i < GVector_Size(windowVector); i++) {
        
            GVectorItem item = GVector_Get(windowVector, i);
            if (item == NULL) {
                return NULL;
            }

            GWindowDef* window = (GWindowDef*)item;
            if (window->redrawFlag) {
                RenderWindow(window);
            }
            
        }

        return;
    }


    XNextEvent(xDisplay, &xEvent);

    GWindowDef* windowDef = TryGetWindow(xEvent.xany.window);

    if (windowDef == NULL) {
        //DEBUG_LOG(FAIL, "Event recieved for unknown window!");
        return;
    }

    // REDRAW WINDOW IF IT NEEDS IT

    clock_t now = clock();

    double timeSinceLastFrame = ((double) (now - lastFrameTime));

    if (xEvent.type == Expose) {
        //printf("EXPOSE\n");

        RenderWindow((GWindow)windowDef);
    } else if (windowDef->redrawFlag && timeSinceLastFrame > 1000000.0/144.0) {
                RenderWindow((GWindow)windowDef);

    }

    switch(xEvent.type) {
        case ConfigureNotify:
            
            int newWidth = xEvent.xconfigure.width;
            int newHeight = xEvent.xconfigure.height;

            if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef->userData, windowDef, newSize);
                }
            }
            
            break;
        case MotionNotify:

            end = clock();
            cpu_time_used = ((double) (end - start));

            //printf("Mouse event took %f us to arrive.\n", cpu_time_used);

   
            //if (cpu_time_used > 1000.0) {
                GWindowPoint motionLocation = {xEvent.xmotion.x, xEvent.xmotion.y};
                if (windowDef->pointerMoveDelegate != NULL) {
                    (windowDef->pointerMoveDelegate)(windowDef->userData, windowDef, motionLocation);
                }

                //start = clock();

            //} 
            

            break;
        case ButtonPress:
        
            GWindowPoint buttonDownLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonDownDelegate != NULL) {
                (windowDef->buttonDownDelegate)(windowDef->userData, windowDef, buttonDownLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case ButtonRelease:

            GWindowPoint buttonUpLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonUpDelegate != NULL) {
                (windowDef->buttonUpDelegate)(windowDef->userData, windowDef, buttonUpLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&xEvent.xkey, eventText, sizeof(eventText), &eventKey, 0);
            printf("Key pressed: %s\n", eventText);
            break;

        case ClientMessage:
            
            if (xEvent.xclient.data.l[0] == (Atom)windowDef->xDeleteAtom) {

                if (windowDef->willCloseDelegate != NULL) {

                    if ((windowDef->willCloseDelegate)(windowDef->userData, (GWindow)windowDef)) {
                        if (windowDef->closeDelegate != NULL) {
                            (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                        }

                        GWindow_Close((GWindow)windowDef);
                    }
                } else {
                    
                    if (windowDef->closeDelegate != NULL) {
                        (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                    }

                    GWindow_Close((GWindow)windowDef);
                }
            }

            break;
    }
}

size_t GWindowDef_NumberOfOpenWindows() {
    return GVector_Size(windowVector);
}

GWindowDef* TryGetWindow(Window xWindow) {

    for (int i = 0; i < GVector_Size(windowVector); i++) {
        
        GVectorItem item = GVector_Get(windowVector, i);
        if (item == NULL) {
            return NULL;
        }

        GWindowDef* window = (GWindowDef*)item;
        if (window->rawHandle == (GVectorItem)xWindow) {
            return (GWindowDef*)GVector_Get(windowVector, i);
        }
    }
        
    return NULL;
}



clock_t then = 0;


void RenderWindow(GWindow window) {

    clock_t now = clock();

    double frameTime = ((double) (now - then));

    then = now;

    lastFrameTime = now;

    printf("FRAME TIME %f us\n", frameTime);
    


    if (!GVector_Contains(windowVector, window)) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    

    GFrameInfo frameInfo = { 
        windowDef->width,
        windowDef->height
    };

    printf("%d %d\n", windowDef->width, windowDef->height);

    GRenderManager_RenderWindow(window);

    /*GFrame frame = GFrame_Alloc(frameInfo);

    GColor color = { 1.0, 1.0, 0.0, 1.0};
    GRect rect = { 0.0, 0.0, 800.0, 600.0};
    GFrame_Fill(frame, rect, color );


    if (windowDef->drawDelegate != NULL) {
        (windowDef->drawDelegate)(windowDef->userData, (GWindow)windowDef, frame);

        
    }
    GFrame_Free(frame);
*/

            
        windowDef->redrawFlag = false;
}