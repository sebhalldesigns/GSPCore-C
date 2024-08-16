#ifndef GSP_WINDOW_X11_H
#define GSP_WINDOW_X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <gsp_window/gsp_window.h>
#include "gsp_window_internal.h"

typedef struct {
    Display* display;
    Window root;
    XEvent event;
    KeySym event_key;
    char event_text[256];
} gwindow_x11_state_t;


gnative_window_t gsp_window_x11_create_window();

void gsp_window_x11_poll_events();

void gsp_window_x11_set_title(gnative_window_t window, gstring_t title);


#endif // GSP_WINDOW_X11_H