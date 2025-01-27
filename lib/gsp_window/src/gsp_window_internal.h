#ifndef GSP_WINDOW_INTERNAL_H
#define GSP_WINDOW_INTERNAL_H

#include <gsp_window/gsp_window.h>

typedef uintptr_t gnative_window_t;

void gsp_window_system_event_callback(gnative_window_t native_window, gwindow_event_t event);

gwindow_t gsp_window_system_resolve_native(gnative_window_t native_window);

#endif // GSP_WINDOW_INTERNAL_H