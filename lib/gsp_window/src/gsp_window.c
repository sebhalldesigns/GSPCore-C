#include <gsp_window/gsp_window.h>
#include "gsp_window_internal.h"

#include <stdio.h>
#include <stdlib.h>

#include <gsp_containers/gsp_list.h>
#include <gsp_debug/gsp_debug.h>
#include <gsp_renderer/gsp_renderer.h>

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

typedef struct {
    gnative_window_t native_window;
    gwindow_event_callback_t callback;
} gwindow_impl_t;

static gsp_window_backend_t backend = BACKEND_NONE;

static glist_t windows = NULL;

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

    return gsp_list_does_node_exist(windows, (glist_node_t)window);

}

gwindow_t gsp_window_create_window() {

    if (NULL == windows) {
        windows = gsp_list_create_list();
    }

    gnative_window_t native_window;

    if (BACKEND_NONE == backend) {

        gsp_debug_log(INFO, "No backend identified, attempting to set up...");

        #ifdef GSPCORE_BUILD_UNIX

            native_window = gsp_window_wayland_create_window();

            if ((gnative_window_t) 0 != native_window) {
                backend = BACKEND_WAYLAND;
            } else {
                native_window = gsp_window_x11_create_window();
                if ((gnative_window_t) 0 != native_window) {
                    backend = BACKEND_X11;
                }
            }
        #elif GSPCORE_BUILD_WIN32

            native_window = gsp_window_win32_create_window();
            
            if ((gnative_window_t) 0 != native_window) {
                backend = BACKEND_WIN32;
            }

        #endif
        
    } else {
        switch (backend) {

            #ifdef GSPCORE_BUILD_UNIX 
                case BACKEND_WAYLAND:
                    native_window = gsp_window_wayland_create_window();
                    break;
                case BACKEND_X11:
                    native_window = gsp_window_x11_create_window();
                    break;
            #elif GSPCORE_BUILD_WIN32
                case BACKEND_WIN32:
                    native_window = gsp_window_win32_create_window();
                    break;
            #endif
            
        }
    }
 
    if ((gnative_window_t)0 != native_window) {
        
        gwindow_impl_t* window_impl = malloc(sizeof(gwindow_impl_t));

        if (window_impl != NULL) {

            window_impl->native_window = native_window;
            window_impl->callback = NULL;

            glist_node_t window = gsp_list_create_node(windows);
            gsp_list_set_node_data(windows, window, (uintptr_t)window_impl);

            gsp_debug_log(INFO, "Created gwindow_t %lu", window);

            return (gwindow_t)window;
        }
    }

    gsp_debug_log(FAIL, "Failed to create gwindow_t");

    return NULL;
}

void gsp_window_set_title(gwindow_t window, gstring_t title) {

    gwindow_impl_t* window_impl = (gwindow_impl_t*)gsp_list_get_node_data(windows, (glist_node_t)window);

    if (NULL == window_impl) {
        return;
    }

    switch (backend) {

        #ifdef GSPCORE_BUILD_UNIX 
            case BACKEND_WAYLAND:
                gsp_window_wayland_set_title(window_impl->native_window, title);
                break;
            case BACKEND_X11:
                gsp_window_x11_set_title(window_impl->native_window, title);
                break;
        #elif GSPCORE_BUILD_WIN32
            case BACKEND_WIN32:
                gsp_window_win32_set_title(window_impl->native_window, title);
                break; 
        #endif
        
    }

}

void gsp_window_set_event_callback(gwindow_t window, gwindow_event_callback_t event_callback) {

    gwindow_impl_t* window_impl = (gwindow_impl_t*)gsp_list_get_node_data(windows, (glist_node_t)window);

    if (NULL == window_impl) {
        return;
    }

    window_impl->callback = event_callback;

    gwindow_event_t event = {0};

    (event_callback)(0, event);

    gsp_debug_log(INFO, "Succesfully bound event callback %lu for gwindow_t %lu", event_callback, window);

}

void gsp_window_system_event_callback(gnative_window_t native_window, gwindow_event_t event) {

    size_t window_count = gsp_list_get_node_count(windows);

    for (size_t i = 0; i < window_count; i++) {
        gwindow_impl_t* window_impl = (gwindow_impl_t*)gsp_list_get_data_at_index(windows, i);
        
        if (NULL != window_impl && native_window == window_impl->native_window) {
            // the window was found, so escape loop and return
            if (window_impl->callback != NULL) {
                printf("CALLING... %lu\n", window_impl->callback);
                (window_impl->callback)(0, event);
                //(window_impl->callback)((gwindow_t)gsp_list_get_node_at_index(windows, i), event);
            }

            return;
        }
    }
    
}

gwindow_t gsp_window_system_resolve_native(gnative_window_t native_window) {
    size_t window_count = gsp_list_get_node_count(windows);

    for (size_t i = 0; i < window_count; i++) {
        gwindow_impl_t* window_impl = (gwindow_impl_t*)gsp_list_get_data_at_index(windows, i);
        
        if (NULL != window_impl && native_window == window_impl->native_window) {
            
            return (gwindow_t)gsp_list_get_node_at_index(windows, i);
        }
    }

    return (gwindow_t)NULL;
}