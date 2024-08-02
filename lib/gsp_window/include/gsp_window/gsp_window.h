#ifndef GSP_WINDOW_H
#define GSP_WINDOW_H

//  gsp_window.h - GSPCore gsp_window library
//  Created by Seb Hall on 23/07/2024
//
//  OpenGSP is a collection of lightweight, cross-platform libraries that provide 
//  functionality related to graphics, user interfaces and data processing.
// 
//  This project is distributed under the MIT license which is as follows:
//      
//      MIT License
//      
//      Copyright (c) 2024 Seb Hall
//      
//      Permission is hereby granted, free of charge, to any person obtaining a copy
//      of this software and associated documentation files (the "Software"), to deal
//      in the Software without restriction, including without limitation the rights
//      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//      copies of the Software, and to permit persons to whom the Software is
//      furnished to do so, subject to the following conditions:
//      
//      The above copyright notice and this permission notice shall be included in all
//      copies or substantial portions of the Software.
//      
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//      SOFTWARE.

#include <gsp_types/gsp_types.h>

#include <stdbool.h>



typedef enum {
    WINDOW_EVENT_CREATE,
    WINDOW_EVENT_DESTROY,
    WINDOW_EVENT_CLOSE_REQUEST,
    WINDOW_EVENT_HIDE,
    WINDOW_EVENT_SHOW,
    WINDOW_EVENT_RESIZE,
    WINDOW_EVENT_MOVE,
    WINDOW_EVENT_POINTER_MOVE,
    WINDOW_EVENT_POINTER_ENTER,
    WINDOW_EVENT_POINTER_EXIT,
    WINDOW_EVENT_KEY_DOWN,
    WINDOW_EVENT_KEY_UP,
    WINDOW_EVENT_BUTTON_DOWN,
    WINDOW_EVENT_BUTTON_UP,
} gwindow_event_type_t;

typedef struct {
    gwindow_event_type_t event_type;
    uint8_t sub_type;
    uint64_t data;
} gwindow_event_t;

typedef (*gwindow_event_callback_t)(gwindow_t window, gwindow_event_t event);

void gsp_window_poll_events();

bool gsp_window_is_window_valid(gwindow_t window);

gwindow_t gsp_window_create_window();

void gsp_window_set_title(gwindow_t window, gstring_t title);

void gsp_window_set_event_callback(gwindow_t window, gwindow_event_callback_t event_callback);


#endif // GSP_WINDOW_H