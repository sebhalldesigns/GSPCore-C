#include <gsp_window/gsp_window.h>
#include "gsp_window_internal.h"

#include <stdio.h>
#include <stdlib.h>

#include <gsp_debug/gsp_debug.h>

#ifdef GSPCORE_BUILD_UNIX   
    
    #include "gsp_window_x11.h"
    #include "gsp_window_wayland.h"

#elif GSPCORE_BUILD_WIN32

    #include "gsp_window_win32.h"

#endif

typedef enum {
    BACKEND_NONE,
    BACKEND_X11,
    BACKEND_WAYLAND,
    BACKEND_WIN32
} gsp_window_backend_t;

static gsp_window_backend_t backend = BACKEND_NONE;

typedef struct linked_window {
    gwindow_t window;
    struct linked_window* next;
    gwindow_event_callback_t callback;
} linked_window_t;

static linked_window_t* first_window = NULL;

void gsp_window_poll_events() {
    switch (backend) {

        #ifdef GSPCORE_BUILD_UNIX 
            case BACKEND_WAYLAND:
                gsp_window_wayland_poll_events();
                break;
            case BACKEND_X11:
                gsp_window_x11_poll_events();
                break;
        #elif GSPCORE_BUILD_WIN32
            case BACKEND_WIN32:
                gsp_window_win32_poll_events();
                break;
        #endif
        
    }
}

bool gsp_window_is_window_valid(gwindow_t window) {

    if (NULL == first_window) {
        // no valid windows open
        return false;
    }

    // iterate over windows linked list

    linked_window_t* last_window = first_window;

    while (last_window != NULL) {
        
        if (window == last_window->window) {
            // the window was found, so escape loop and return true
            return true;
        }

        last_window = last_window->next;
    }
    
    // only get to here if have gone through the whole linked list and not found the window
    return false;
}

gwindow_t gsp_window_create_window() {

    gwindow_t window;

    if (BACKEND_NONE == backend) {

        gsp_debug_log(INFO, "No backend identified, attempting to set up...");

        #ifdef GSPCORE_BUILD_UNIX

            window = gsp_window_wayland_create_window();

            if ((gwindow_t) 0 != window) {
                backend = BACKEND_WAYLAND;
            } else {
                window = gsp_window_x11_create_window();
                if ((gwindow_t) 0 != window) {
                    backend = BACKEND_X11;
                }
            }
        #elif GSPCORE_BUILD_WIN32

            window = gsp_window_win32_create_window();
            
            if ((gwindow_t) 0 != window) {
                backend = BACKEND_WIN32;
            }

        #endif
        

    } else {
        switch (backend) {

            #ifdef GSPCORE_BUILD_UNIX 
                case BACKEND_WAYLAND:
                    window = gsp_window_wayland_create_window();
                    break;
                case BACKEND_X11:
                    window = gsp_window_x11_create_window();
                    break;
            #elif GSPCORE_BUILD_WIN32
                case BACKEND_WIN32:
                    window = gsp_window_win32_create_window();
                    break;
            #endif
            
        }
    }
 
    if (window != (gwindow_t) 0) {
        gsp_debug_log(INFO, "Window created successfully");

        linked_window_t* new_window = (linked_window_t*) malloc(sizeof(linked_window_t));
        
        if (NULL == new_window) {
            // linked window allocation failed, return an invalid window
            return (gwindow_t) 0;
        }

        new_window->window = window;
        new_window->next = NULL;


        // insert into linked list
        if (NULL == first_window) {

            // set as the first window
            first_window = new_window;

        } else {
            
            // add to the end of the linked list

            linked_window_t* last_window = first_window;

            while (last_window->next != NULL) {
                last_window = last_window->next;
            }

            last_window->next  = new_window;

        }
    }

    return window;
}

void gsp_window_set_title(gwindow_t window, gstring_t title) {

    // escape function if invalid window
    if (!gsp_window_is_window_valid(window)) {
        return;
    }

    switch (backend) {

        #ifdef GSPCORE_BUILD_UNIX 
            case BACKEND_WAYLAND:
                gsp_window_wayland_set_title(window, title);
                break;
            case BACKEND_X11:
                gsp_window_x11_set_title(window, title);
                break;
        #elif GSPCORE_BUILD_WIN32
            case BACKEND_WIN32:
                gsp_window_win32_set_title(window, title);
                break; 
        #endif
        
    }

}

void gsp_window_set_event_callback(gwindow_t window, gwindow_event_callback_t event_callback) {

    if (NULL == first_window) {
        // no valid windows open
        return false;
    }

    // iterate over windows linked list

    linked_window_t* last_window = first_window;

    while (last_window != NULL) {
        
        if (window == last_window->window) {
            // the window was found, so escape loop and return
            last_window->callback = event_callback;
            gsp_debug_log(INFO, "Succesfully bound event callback for window");
            return;
        }

        last_window = last_window->next;
    }

}

void gsp_window_system_event_callback(gwindow_t window, gwindow_event_t event) {

    if (NULL == first_window) {
        // no valid windows open
        return false;
    }

    // iterate over windows linked list

    linked_window_t* last_window = first_window;

    while (last_window != NULL) {
        
        if (window == last_window->window) {
            // the window was found, so escape loop and return
            if (last_window->callback != NULL) {
                (last_window->callback)(window, event);
            }
            
            return;
        }

        last_window = last_window->next;
    }
    
}