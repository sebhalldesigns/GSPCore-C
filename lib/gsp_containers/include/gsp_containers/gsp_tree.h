#ifndef GSP_TREE_H
#define GSP_TREE_H

//  gsp_tree.h - GSPCore gsp_containers library
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


GSPCORE_API gtree_node_t gsp_tree_create_root_node();
GSPCORE_API gtree_node_t gsp_tree_create_sibling_node(gtree_node_t root);
GSPCORE_API gtree_node_t gsp_tree_create_child_node(gtree_node_t parent);

GSPCORE_API void gsp_tree_destroy_node(gtree_node_t node);
GSPCORE_API bool gsp_tree_check_node_exists(gtree_node_t node);

GSPCORE_API void gsp_tree_set_node_data(gtree_node_t node, uintptr_t data);
GSPCORE_API uintptr_t gsp_tree_get_node_data(gtree_node_t node);

#endif // GSP_TREE_H
