#ifndef GSP_WINDOW_WAYLAND_H
#define GSP_WINDOW_WAYLAND_H

#include <gsp_window/gsp_window.h>

#include <stdbool.h>

#include "xdg-shell-client-header.h"
#include "kde-server-decoration.h"


enum pointer_event_mask {
       POINTER_EVENT_ENTER = 1 << 0,
       POINTER_EVENT_LEAVE = 1 << 1,
       POINTER_EVENT_MOTION = 1 << 2,
       POINTER_EVENT_BUTTON = 1 << 3,
       POINTER_EVENT_AXIS = 1 << 4,
       POINTER_EVENT_AXIS_SOURCE = 1 << 5,
       POINTER_EVENT_AXIS_STOP = 1 << 6,
       POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};

struct pointer_event {
       uint32_t event_mask;
       wl_fixed_t surface_x, surface_y;
       uint32_t button, state;
       uint32_t time;
       uint32_t serial;
       struct {
               bool valid;
               wl_fixed_t value;
               int32_t discrete;
       } axes[2];
       uint32_t axis_source;
};

typedef struct {
    struct wl_display* display;
    struct wl_registry* registry;
    struct wl_compositor* compositor;
    struct wl_shm* shm;
    struct xdg_wm_base* xd_wm_base;
    struct org_kde_kwin_server_decoration_manager* kde_decorations;

    struct wl_seat* seat;
    struct wl_keyboard* keyboard;
    struct wl_pointer* pointer;
    struct wl_touch* touch;

    struct pointer_event pointer_event;
} gwindow_wayland_state_t;

gwindow_t gsp_window_wayland_create_window();

void gsp_window_wayland_poll_events();

void gsp_window_wayland_set_title(gwindow_t window, gstring_t title);


#endif // GSP_WINDOW_WAYLAND_H