#ifndef GSP_VIEW_H
#define GSP_VIEW_H

//  gsp_view.h - GSPCore gsp_view library
//  Created by Seb Hall on 14/08/2024
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

#ifdef _WIN32
    #ifdef GSPCORE_BUILD_WIN32
        #define GSPCORE_API __declspec(dllexport)
    #else
        #define GSPCORE_API __declspec(dllimport)
    #endif
#else
    #define GSPCORE_API
#endif


const static uint8_t VIEW_LAYOUT_NONE = 0x00;
const static uint8_t VIEW_LAYOUT_STACK = 0x01;
const static uint8_t VIEW_LAYOUT_DOCK = 0x02;
const static uint8_t VIEW_LAYOUT_GRID = 0x03;
const static uint8_t VIEW_LAYOUT_WRAP = 0x04;

const static uint8_t VIEW_DIRECTION_HORIZONTAL = 0x00;
const static uint8_t VIEW_DIRECTION_VERTICAL = 0x01;

const static uint8_t VIEW_DOCK_TOP = 0x00;
const static uint8_t VIEW_DOCK_LEFT = 0x01;
const static uint8_t VIEW_DOCK_RIGHT = 0x02;
const static uint8_t VIEW_DOCK_BOTTOM = 0x03;


// CALLED BY SYSTEM
GSPCORE_API gview_t gsp_view_create_view();

// destroys view and all subviews
GSPCORE_API void gsp_view_destroy_view(gview_t view);

GSPCORE_API void gsp_view_layout_window(gwindow_t window, gsize_t size);

// check if view exists
GSPCORE_API bool gsp_view_is_view_valid(gview_t view);
GSPCORE_API bool gsp_view_is_view_root(gview_t view);
GSPCORE_API gview_t gsp_view_get_window_root(gwindow_t window);


GSPCORE_API void gsp_view_set_window_root(gwindow_t window, gview_t view);
GSPCORE_API void gsp_view_add_child(gview_t parent, gview_t child);
GSPCORE_API void gsp_view_children_count(gview_t parent, gview_t child);
GSPCORE_API void gsp_view_set_background_color(gview_t view, gcolor_t color);


GSPCORE_API void gsp_view_set_view_layout(gview_t view, uint8_t layout);

GSPCORE_API void gsp_view_set_stack_direction(gview_t view, uint8_t direction);
GSPCORE_API void gsp_view_set_grid_size(gview_t view, size_t rows, size_t columns);
GSPCORE_API void gsp_view_set_wrap_direction(gview_t view, uint8_t direction);

GSPCORE_API void gsp_view_set_dock_request(gview_t view, uint8_t request);
GSPCORE_API void gsp_view_set_grid_request(gview_t view, size_t row, size_t column);

GSPCORE_API void gsp_view_set_size_request(gview_t view, size_t row, size_t column);

#endif // GSP_VIEW_H
