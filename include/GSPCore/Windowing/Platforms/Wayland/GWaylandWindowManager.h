#ifndef GWAYLANDWINDOWMANAGER_H
#define GWAYLANDWINDOWMANAGER_H

#include <stdbool.h>

#include "xdg-shell-client-header.h"
#include "kde-server-decoration.h"

typedef struct {
    struct wl_registry* registry;
    struct wl_compositor* compositor;
    struct wl_shm* shm;
    struct xdg_wm_base* xd_wm_base;
    struct org_kde_kwin_server_decoration_manager* kde_decorations;
} GWaylandState;



bool GWaylandWindowManager_TryInit();
GWindow* GWaylandWindowManager_OpenWindow();
int GWaylandWindowManager_RunLoop();

void GWaylandWindowManager_GlobalRegistryHandle(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
void GWaylandWindowManager_GlobalRegistryRemoveHandle(void *data, struct wl_registry *registry, uint32_t name);
void GWaylandWindowManager_XdgSurfaceConfigureCallback(void *data, struct xdg_surface *xdg_surface, uint32_t serial);
void GWaylandWindowManager_TopLevelConfigureCallback(void* data, struct xdg_toplevel* toplevel, int32_t width, int32_t height, struct wl_array* states);
void GWaylandWindowManager_TopLevelCloseCallback(void* data, struct xdg_toplevel* toplevel);


#endif // GWAYLANDWINDOWMANAGER_H