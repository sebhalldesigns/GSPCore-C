#include <gsp_view/gsp_view.h>

#include <gsp_debug/gsp_debug.h>
#include <gsp_containers/gsp_list.h>

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    NONE,
    STACK,
    DOCK,
    GRID,
    WRAP
} gview_layout_t;

typedef enum {
    HORIZONTAL,
    VERTICAL
} gview_direction_t;


typedef enum {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
} gview_dock_t;

typedef union {
    
    struct {
        gview_direction_t direction;
    } stack_info;
    
    struct {
        size_t grid_rows;
        size_t grid_columns;
    } grid_info;

    struct {
        gview_direction_t direction;
    } wrap_info;
    
} gview_parent_layout_info_t;

typedef union {
    
    struct {
        gview_dock_t dock_request;
    } dock_info;
    
    struct {
        size_t row_request;
        size_t column_request;
    } grid_info;

} gview_child_layout_info_t;

typedef struct {
    gcolor_t background;
    gsize_t size_request;
} gsurface_contents_t;

typedef struct {
    gcolor_t background_color;
    
    gview_layout_t layout;
    gview_parent_layout_info_t parent_layout_info;

    gview_child_layout_info_t child_layout_info;
    
    glist_t children;

    gsize_t requested_size;
    grect_t calculated_rect;
} gsp_view_impl_t;

static glist_t root_list = NULL;

gview_t gsp_view_create_view() {

    if (NULL == root_list) {
        root_list = gsp_list_create_list();

        if (NULL == root_list) {
            return NULL;
        }
    }

    gsp_view_impl_t* view = (gsp_view_impl_t*)malloc(sizeof(gsp_view_impl_t));

    if (NULL == view) {
        return NULL;
    }

    // add view to top level
    glist_node_t view_node = gsp_list_create_node(root_list);

    if (NULL == view_node) {
        return NULL;
    }

    gsp_list_set_node_data(root_list, view_node, (uintptr_t)view_node);

    gsp_debug_log(INFO, "Allocated gview_t %lu", view);

    return (gview_t)view;
}

gview_t gsp_view_destroy_view() {
    
   
}

bool gsp_view_is_view_valid() {

    /*if (NULL == root_list) {
        // no views exist
        return false;
    }

    glist_t stack = gsp_list_create_list(); 

    glist_node_t root = gsp_list_create_node(stack);
    gsp_list_set_node_data(stack, root, root_list);

    while (gsp_list_get_node_count(stack) > 0) {

        size_t count = gsp_list_get_node_count(stack);

        // pop highest list off stack
        glist_t current = gsp_list_get_data_at_index(stack, count - 1);
        gsp_list_destroy_node(stack, gsp_list_get_node_at_index(stack, count - 1));

        if (current)
        

    }*/
    
}

void gsp_view_set_window_root_view(gwindow_t window, gview_t view) {

}

void gsp_view_add_child(gview_t parent, gview_t child) {

}

void gsp_view_children_count(gview_t parent, gview_t child) {

}

void gsp_view_set_view_background_color(gview_t view, gcolor_t color) {

}

void gsp_view_set_view_layout(gview_t view, uint8_t layout) {

}

void gsp_view_set_stack_direction(gview_t view, uint8_t direction) {

}

void gsp_view_set_grid_size(gview_t view, size_t rows, size_t columns) {

}

void gsp_view_set_wrap_direction(gview_t view, uint8_t direction) {

}

void gsp_view_set_dock_request(gview_t view, uint8_t request) {

}

void gsp_view_set_grid_request(gview_t view, size_t row, size_t column) {

}

void gsp_view_set_size_request(gview_t view, size_t row, size_t column) {

}
