#ifndef GSP_WINDOW_H
#define GSP_WINDOW_H

#include <gsp_types/gsp_types.h>

#include <stdbool.h>


void gsp_window_poll_events();

bool gsp_window_is_window_valid(gwindow_t window);

gwindow_t gsp_window_create_window();

void gsp_window_set_title(gwindow_t window, gstring_t title);


#endif // GSP_WINDOW_H