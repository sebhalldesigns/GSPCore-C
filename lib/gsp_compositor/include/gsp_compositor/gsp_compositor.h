#ifndef GSP_COMPOSITOR_H
#define GSP_COMPOSITOR_H

//  gsp_compositor.h - GSPCore gsp_compositor library
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

const static uint8_t COMPOSITOR_LAYOUT_NONE = 0x01;
const static uint8_t COMPOSITOR_LAYOUT_STACK = 0x02;
const static uint8_t COMPOSITOR_LAYOUT_DOCK = 0x03;
const static uint8_t COMPOSITOR_LAYOUT_GRID = 0x04;
const static uint8_t COMPOSITOR_LAYOUT_WRAP = 0x05;

const static uint8_t COMPOSITOR_DIRECTION_HORIZONTAL = 0x01;
const static uint8_t COMPOSITOR_DIRECTION_VERTICAL = 0x02;

const static uint8_t COMPOSITOR_DOCK_TOP = 0x01;
const static uint8_t COMPOSITOR_DOCK_LEFT = 0x02;
const static uint8_t COMPOSITOR_DOCK_RIGHT = 0x03;
const static uint8_t COMPOSITOR_DOCK_BOTTOM = 0x04;


GSPCORE_API gsurface_t gsp_compositor_create_surface();

GSPCORE_API void gsp_compositor_set_surface_background(gsurface_t surface, gcolor_t background);

GSPCORE_API void gsp_compositor_add_child_surface(gsurface_t surface, gcolor_t background);

GSPCORE_API void gsp_compositor_set_window_root_surface(gwindow_t window, gsurface_t surface);

GSPCORE_API void gsp_compositor_set_surface_layout(gsurface_t surface, uint8_t layout);

GSPCORE_API void gsp_compositor_set_surface_size_request(gsurface_t surface, gsize_t size);

// set layout affecting children

GSPCORE_API void gsp_compositor_set_stack_direction(gsurface_t surface, uint8_t direction);

GSPCORE_API void gsp_compositor_set_grid_size(gsurface_t surface, size_t rows, size_t columns);

GSPCORE_API void gsp_compositor_set_wrap_direction(gsurface_t surface, uint8_t direction);

// set layout affecting itself

GSPCORE_API void gsp_compositor_set_dock_request(gsurface_t surface, uint8_t request);

GSPCORE_API void gsp_compositor_set_grid_request(gsurface_t surface, size_t row, size_t column);

 



#endif // GSP_COMPOSITOR_H
