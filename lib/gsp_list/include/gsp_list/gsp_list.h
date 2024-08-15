#ifndef GSP_LIST_H
#define GSP_LIST_H

//  gsp_list.h - GSPCore gsp_list library
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


// note - any node is a member of exactly one list. When a list is destroyed, so are all nodes.

GSPCORE_API glist_t gsp_list_create_list();
GSPCORE_API void gsp_list_destroy_list(glist_t list);

GSPCORE_API gnode_t gsp_list_create_node(glist_t list);
GSPCORE_API void gsp_list_destroy_node(glist_t list, gnode_t node);

GSPCORE_API bool gsp_list_does_list_exist(glist_t list);
GSPCORE_API bool gsp_list_does_node_exist(glist_t list, gnode_t node);

GSPCORE_API size_t gsp_list_get_node_count(glist_t list);

GSPCORE_API uintptr_t gsp_list_get_node_data(glist_t list, gnode_t node);
GSPCORE_API void gsp_list_set_node_data(glist_t list, gnode_t node, uintptr_t data);

#endif // GSP_LIST_H
