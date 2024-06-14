#include <GSPCore/GSPCore.h>
#include <GSPCore/Windowing/Platforms/Wayland/GWaylandWindowManager.h>
#include <GSPCore/Windowing/Platforms/Wayland/xdg-shell-client-header.h>
#include <GSPCore/Windowing/Platforms/Wayland/kde-server-decoration.h>
#include <GSPCore/Windowing/GWindow.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Wayland headers
#include <wayland-client.h>
#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

static struct wl_display* waylandDisplay = NULL;
static GWaylandState waylandState;


const struct wl_registry_listener registry_listener = {
	.global = GWaylandWindowManager_GlobalRegistryHandle,
	.global_remove = GWaylandWindowManager_GlobalRegistryRemoveHandle,
};

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = GWaylandWindowManager_XdgSurfaceConfigureCallback,
};


static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};


bool GWaylandWindowManager_TryInit() {



    waylandDisplay = wl_display_connect(NULL);

    if (waylandDisplay == NULL) {
        GLog(WARNING, "No Wayland display found. Falling back to X11");
        return false;
    }



    
    waylandState.registry = wl_display_get_registry(waylandDisplay);
    wl_registry_add_listener(waylandState.registry, &registry_listener, &waylandState);
	wl_display_roundtrip(waylandDisplay);


    GLog(INFO, "Started WindowManager on Wayland backend");

    return true;
}   

GWindow GWaylandWindowManager_OpenWindow() {
    GWindow window;
    window.platformHandles.surface = wl_compositor_create_surface(waylandState.compositor);
    window.platformHandles.xd_surface = xdg_wm_base_get_xdg_surface(waylandState.xd_wm_base, window.platformHandles.surface);
    xdg_surface_add_listener(window.platformHandles.xd_surface, &xdg_surface_listener, &waylandState);
    window.platformHandles.xd_toplevel = xdg_surface_get_toplevel(window.platformHandles.xd_surface);

    xdg_toplevel_set_title(window.platformHandles.xd_toplevel, "Example client");
    org_kde_kwin_server_decoration_manager_create(waylandState.kde_decorations, window.platformHandles.surface);

    wl_surface_commit(window.platformHandles.surface);

    while (wl_display_dispatch(waylandDisplay)) {
        /* This space deliberately left blank */
    }

    wl_display_disconnect(waylandDisplay);
}


void GWaylandWindowManager_GlobalRegistryHandle(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    printf("interface: '%s', version: %d, name: %d\n", interface, version, name);

    GWaylandState* state = (GWaylandState*)(data);

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = (struct wl_compositor*)(wl_registry_bind(state->registry, name, &wl_compositor_interface, 4));
    }

    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->shm = (struct wl_shm*)(wl_registry_bind(state->registry, name, &wl_shm_interface, 1));
    }

    if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xd_wm_base = (struct xdg_wm_base*)(wl_registry_bind(state->registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(state->xd_wm_base,
                &xdg_wm_base_listener, state);
    }

    if (strcmp(interface, org_kde_kwin_server_decoration_manager_interface.name) == 0) {
        state->kde_decorations = (struct org_kde_kwin_server_decoration_manager*)(wl_registry_bind(state->registry, name, &org_kde_kwin_server_decoration_manager_interface, 1));
    }
}

void GWaylandWindowManager_GlobalRegistryRemoveHandle(void *data, struct wl_registry *registry, uint32_t name) {

}

void GWaylandWindowManager_XdgSurfaceConfigureCallback(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);

    //struct wl_buffer* buffer = draw_frame(state);
    //wl_surface_attach(waylandState.surface, buffer, 0, 0);
    //wl_surface_commit(xdg_surface);
}
