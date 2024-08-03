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


// basic window events
const static uint8_t WINDOW_EVENT_CREATE = 0x01;
const static uint8_t WINDOW_EVENT_DESTROY = 0x02;
const static uint8_t WINDOW_EVENT_MOVE = 0x03;
const static uint8_t WINDOW_EVENT_RESIZE = 0x04;
const static uint8_t WINDOW_EVENT_ACTIVATE = 0x05;
const static uint8_t WINDOW_EVENT_DEACTIVATE = 0x06;
const static uint8_t WINDOW_EVENT_CLOSE_REQUEST = 0x07;
const static uint8_t WINDOW_EVENT_HIDE = 0x08;
const static uint8_t WINDOW_EVENT_SHOW = 0x09;

// pointer and button events
const static uint8_t WINDOW_EVENT_POINTER_MOVE = 0x20;
const static uint8_t WINDOW_EVENT_POINTER_ENTER = 0x21;
const static uint8_t WINDOW_EVENT_POINTER_EXIT = 0x22;
const static uint8_t WINDOW_EVENT_BUTTON_DOWN = 0x23; // sub button #
const static uint8_t WINDOW_EVENT_BUTTON_UP = 0x24; // sub button #
const static uint8_t WINDOW_EVENT_SCROLL = 0x25; // sub axis

// keyboard events
const static uint8_t WINDOW_EVENT_KEY_DOWN = 0x30; // raw scancode
const static uint8_t WINDOW_EVENT_KEY_UP = 0x31; // raw 
const static uint8_t WINDOW_EVENT_CHAR = 0x32; // UTF-8 character, sub number of bytes
const static uint8_t WINDOW_EVENT_TYPE_MOD = 0x33; // typing modifiers such as delete, arrow keys etc

// system events
const static uint8_t WINDOW_EVENT_SYSTEM_COLOR_CHANGE = 0x40; 
const static uint8_t WINDOW_EVENT_SYSTEM_SCALING_CHANGE = 0x41;

typedef struct {
    uint8_t event_id;
    uint8_t sub_id;
    uint64_t data;
} gwindow_event_t;

typedef (*gwindow_event_callback_t)(gwindow_t window, gwindow_event_t event);

void gsp_window_poll_events();

bool gsp_window_is_window_valid(gwindow_t window);

gwindow_t gsp_window_create_window();

void gsp_window_set_title(gwindow_t window, gstring_t title);

void gsp_window_set_event_callback(gwindow_t window, gwindow_event_callback_t event_callback);


#endif // GSP_WINDOW_H