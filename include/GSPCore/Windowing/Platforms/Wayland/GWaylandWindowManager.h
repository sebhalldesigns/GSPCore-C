#ifndef GWAYLANDWINDOWMANAGER_H
#define GWAYLANDWINDOWMANAGER_H

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
} GWaylandState;

bool GWaylandWindowManager_TryInit();
GWindow* GWaylandWindowManager_OpenWindow();
int GWaylandWindowManager_RunLoop();

void GWaylandWindowManager_GlobalRegistryHandle(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
void GWaylandWindowManager_GlobalRegistryRemoveHandle(void *data, struct wl_registry *registry, uint32_t name);
void GWaylandWindowManager_XdgSurfaceConfigureCallback(void *data, struct xdg_surface *xdg_surface, uint32_t serial);
void GWaylandWindowManager_TopLevelConfigureCallback(void* data, struct xdg_toplevel* toplevel, int32_t width, int32_t height, struct wl_array* states);
void GWaylandWindowManager_TopLevelCloseCallback(void* data, struct xdg_toplevel* toplevel);

void GWaylandWindowManager_PointerEnterCallback(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
void GWaylandWindowManager_PointerExitCallback(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface);
void GWaylandWindowManager_PointerMotionCallback(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
void GWaylandWindowManager_PointerButtonCallback(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);

void GWaylandWindowManager_PointerAxisCallback(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
void GWaylandWindowManager_PointerFrameCallback(void *data, struct wl_pointer *wl_pointer);
void GWaylandWindowManager_PointerAxisSourceCallback(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source);
void GWaylandWindowManager_PointerAxisStopCallback(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis);
void GWaylandWindowManager_PointerAxisDiscreteCallback(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete);

#endif // GWAYLANDWINDOWMANAGER_H