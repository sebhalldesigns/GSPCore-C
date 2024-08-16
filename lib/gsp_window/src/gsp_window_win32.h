#ifndef GSP_WINDOW_WIN32_H
#define GSP_WINDOW_WIN32_H

#include <Windows.h>
#include <GL/gl.h>
#include "wglext.h"

#include <gsp_window/gsp_window.h>
#include "gsp_window_internal.h"

typedef struct {
    HINSTANCE instance;
    MSG msg;
} gwindow_win32_state_t;

gnative_window_t gsp_window_win32_create_window();

void gsp_window_win32_poll_events();

void gsp_window_win32_set_title(gnative_window_t window, gstring_t title);



#endif // GSP_WINDOW_WIN32_H