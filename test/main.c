#include <gsp_window/gsp_window.h>
#include <stdio.h>

void event_callback(gwindow_t window, gwindow_event_t event) {
    if (event.event_id == WINDOW_EVENT_RESIZE) {
        printf("%llu to %f %f\n", window, (float)(event.data & 0xFFFFFFFF) , (float)((event.data >> 32) & 0xFFFFFFFF));
    }
}

int main() {

    gwindow_t window1 = gsp_window_create_window();
    gwindow_t window2 = gsp_window_create_window();

    gsp_window_set_title(window1, "WINDOW 1");
    gsp_window_set_title(window2, "WINDOW 2");

    gsp_window_set_event_callback(window1, event_callback);
    gsp_window_set_event_callback(window2, event_callback);

    while (true) {
        gsp_window_poll_events();
    }

    return 0;
}