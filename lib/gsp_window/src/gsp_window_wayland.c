#include "gsp_window_wayland.h"

#include <gsp_debug/gsp_debug.h>

#include <stdio.h>

// Wayland headers
#include <wayland-client.h>
#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

static gwindow_wayland_state_t state;

/*
const struct wl_registry_listener registry_listener = {
	.global = GWaylandWindowManager_GlobalRegistryHandle,
	.global_remove = GWaylandWindowManager_GlobalRegistryRemoveHandle,
};

static const struct xdg_toplevel_listener toplevelListener = {
    .configure = GWaylandWindowManager_TopLevelConfigureCallback,
    .close = GWaylandWindowManager_TopLevelCloseCallback,
};

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = GWaylandWindowManager_XdgSurfaceConfigureCallback,
};

static const struct wl_pointer_listener wl_pointer_listener = {
       .enter = GWaylandWindowManager_PointerEnterCallback,
       .leave = GWaylandWindowManager_PointerExitCallback,
       .motion = GWaylandWindowManager_PointerMotionCallback,
       .button = GWaylandWindowManager_PointerButtonCallback,
       .axis = GWaylandWindowManager_PointerAxisCallback,
       .frame = GWaylandWindowManager_PointerFrameCallback,
       .axis_source = GWaylandWindowManager_PointerAxisSourceCallback,
       .axis_stop = GWaylandWindowManager_PointerAxisStopCallback,
       .axis_discrete = GWaylandWindowManager_PointerAxisDiscreteCallback,
};


static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};



static void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
       bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

       if (have_pointer && state.pointer == NULL) {
               state.pointer = wl_seat_get_pointer(state.seat);
               wl_pointer_add_listener(state.pointer,
                               &wl_pointer_listener, &state);
       } else if (!have_pointer && state.pointer != NULL) {
               wl_pointer_release(state.pointer);
               state.pointer = NULL;
       }
}

static void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
       //fprintf(stderr, "seat name: %s\n", name);
}

static const struct wl_seat_listener wl_seat_listener = {
       .capabilities = wl_seat_capabilities,
       .name = wl_seat_name,
};

*/


gwindow_t gsp_window_wayland_create_window() {

    if (state.display == NULL) {

        gsp_debug_log(INFO, "Starting Wayland..");

        state.display = wl_display_connect(NULL);

        if (state.display == NULL) {
            gsp_debug_log(WARNING, "Failed to connect to Wayland server!");
            return (gwindow_t) 0;
        }

    }

    return (gwindow_t) 0;
}

void gsp_window_wayland_poll_events() {

}

void gsp_window_wayland_set_title(gwindow_t window, gstring_t title) {

}