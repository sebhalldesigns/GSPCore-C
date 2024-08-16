#include "gsp_window_x11.h"

#include <gsp_debug/gsp_debug.h>

#include <stdio.h>

static gwindow_x11_state_t state;

gnative_window_t gsp_window_x11_create_window() {

    if (state.display == NULL) {

        gsp_debug_log(INFO, "Starting X11..");

        state.display = XOpenDisplay(NULL);
        state.root = DefaultRootWindow(state.display);

        if (state.display == NULL || state.root == 0) {
            gsp_debug_log(FAIL, "Failed to connect to X11 server!");
            return (gnative_window_t) 0;
        }

    }

    Window x11_window = XCreateSimpleWindow(state.display, state.root, 0, 0, 100, 100, 0, 0, 0);
    XMapWindow(state.display, x11_window);

    return (gnative_window_t) x11_window;
}

void gsp_window_x11_poll_events() {

    if (!XPending(state.display)) {  
        // possibly redraw here
        
    }

    XNextEvent(state.display, &state.event);
}

void gsp_window_x11_set_title(gnative_window_t window, gstring_t title) {

    XStoreName(state.display, (Window)window, title);

}