#include <GSPCore/GSPCore.h>
#include <GSPCore/Windowing/Platforms/Wayland/GWaylandWindowManager.h>
#include <GSPCore/Windowing/Platforms/Wayland/xdg-shell-client-header.h>
#include <GSPCore/Windowing/Platforms/Wayland/kde-server-decoration.h>
#include <GSPCore/Windowing/GWindow.h>

#include <GSPCore/Graphics/GRenderManager.h>

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

static GWaylandState waylandState;
static GWindow* rootWindow = NULL;


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


static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};


bool GWaylandWindowManager_TryInit() {



    waylandState.display = wl_display_connect(NULL);

    if (waylandState.display == NULL) {
        GLog(WARNING, "No Wayland display found. Falling back to X11");
        return false;
    }



    
    waylandState.registry = wl_display_get_registry(waylandState.display);
    wl_registry_add_listener(waylandState.registry, &registry_listener, &waylandState);
	wl_display_roundtrip(waylandState.display);


    GLog(INFO, "Started WindowManager on Wayland backend");

    return true;
}   

GWindow* GWaylandWindowManager_OpenWindow() {
    GWindow* window = calloc(1, sizeof(GWindow));
    window->platformHandles.wl_display = waylandState.display;
    window->platformHandles.wl_surface = wl_compositor_create_surface(waylandState.compositor);
    window->platformHandles.xd_surface = xdg_wm_base_get_xdg_surface(waylandState.xd_wm_base, window->platformHandles.wl_surface);
    xdg_surface_add_listener(window->platformHandles.xd_surface, &xdg_surface_listener, &waylandState);
    window->platformHandles.xd_toplevel = xdg_surface_get_toplevel(window->platformHandles.xd_surface);

    
    xdg_toplevel_set_title(window->platformHandles.xd_toplevel, "Example client");
    org_kde_kwin_server_decoration_manager_create(waylandState.kde_decorations, window->platformHandles.wl_surface);
    xdg_toplevel_add_listener(window->platformHandles.xd_toplevel , &toplevelListener, NULL);

    wl_surface_commit(window->platformHandles.wl_surface);
    wl_display_roundtrip(waylandState.display);

    printf("WAYLAND WINDOW MANAGER DONE");

    //while (wl_display_dispatch(waylandDisplay)) {
    //    /* This space deliberately left blank */
    //}

    //wl_display_disconnect(waylandDisplay);

    window->frame.size.width = 100.0;
    window->frame.size.height = 100.0;
    rootWindow = window;
    return window;
}

int GWaylandWindowManager_RunLoop() {

    GRenderManager_RenderWindow(rootWindow);

    while (true) {
        //printf("RUNNING\n");
        //GRenderManager_RenderWindow(rootWindow);
        wl_display_roundtrip(waylandState.display);
        wl_display_dispatch(waylandState.display);

        if (rootWindow != NULL) {
            GRenderManager_RenderWindow(rootWindow);
        }

    }
   
    wl_display_disconnect(waylandState.display);
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
    
    
    printf("CONFIGURE CALLBACK\n");
}

void GWaylandWindowManager_TopLevelConfigureCallback(void* data, struct xdg_toplevel* toplevel, int32_t width, int32_t height, struct wl_array* states) {
    
    if (rootWindow != NULL) {
            rootWindow->frame.size.width = width;
            rootWindow->frame.size.height = height;
    }
    

}

void GWaylandWindowManager_TopLevelCloseCallback(void* data, struct xdg_toplevel* toplevel) {
    printf("CLOSE REQUESTED!\n");
}