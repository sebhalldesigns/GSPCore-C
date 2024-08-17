#ifndef GSP_RENDERER_H
#define GSP_RENDERER_H

//  gsp_renderer.h - GSPCore gsp_renderer library
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

// CALLED BY SYSTEM
GSPCORE_API void gsp_renderer_set_context(gwindow_t window, grenderer_context_t context);

// CALLED BY SYSTEM
// destroy all objects created with context
GSPCORE_API void gsp_renderer_cleanup_context(gwindow_t window, grenderer_context_t context);
GSPCORE_API void gsp_renderer_set_viewport(gwindow_t window, grenderer_context_t context, gsize_t size);
GSPCORE_API void gsp_renderer_clear(gcolor_t color);

GSPCORE_API grenderer_surface_t gsp_renderer_create_surface(gwindow_t window);
GSPCORE_API void gsp_renderer_set_surface_frame(gwindow_t window, grenderer_surface_t surface, grect_t frame);
GSPCORE_API void gsp_renderer_set_surface_background(gwindow_t window, grenderer_surface_t surface, gcolor_t background_color);

GSPCORE_API void gsp_renderer_upload_surface_texture(gwindow_t window, grenderer_surface_t surface, uint32_t width, uint32_t height, uint8_t* data);


#endif // GSP_RENDERER_H
