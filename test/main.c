#include <gsp_window/gsp_window.h>


int main() {

    gwindow_t window1 = gsp_window_create_window();
    gwindow_t window2 = gsp_window_create_window();

    gsp_window_set_title(window1, "WINDOW 1");
    gsp_window_set_title(window2, "WINDOW 2");

    
    while (true) {
        gsp_window_poll_events();
    }

    return 0;
}