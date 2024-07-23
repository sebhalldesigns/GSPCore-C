#ifndef GSP_DEBUG_H
#define GSP_DEBUG_H

//  gsp_debug.h - GSPCore gsp_debug library
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

#ifdef DEBUG 
#define DEBUG_LOG(level, ...) gsp_debug_log(level, __VA_ARGS__)
#else
#define DEBUG_LOG(level, ...) ((void)0)
#endif

typedef enum {
    INFO,
    WARNING,
    FAIL
} gsp_debug_level_t;


void gsp_debug_log(gsp_debug_level_t level, gstring_t message, ...);


#endif // GSP_DEBUG_H