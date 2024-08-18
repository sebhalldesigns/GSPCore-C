#include <gsp_view/gsp_view.h>

#include <gsp_debug/gsp_debug.h>
#include <gsp_containers/gsp_tree.h>

#include <gsp_renderer/gsp_renderer.h>


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

typedef struct gsp_view_impl_t {

    // 4-way navigation much faster but uses 16 bytes more per view
    struct gsp_view_impl_t* previous_sibling;
    struct gsp_view_impl_t* next_sibling;
    struct gsp_view_impl_t* parent;
    struct gsp_view_impl_t* child;

    gwindow_t window;
    gcolor_t background_color;
    
    gview_layout_t layout;
    gview_parent_layout_info_t parent_layout_info;

    gview_child_layout_info_t child_layout_info;
    
    gsize_t requested_size;
    grect_t calculated_rect;

    uint32_t texture;

} gsp_view_impl_t;

static gsp_view_impl_t* root_view = NULL;

gview_t gsp_view_create_view() {


    gsp_view_impl_t* view = (gsp_view_impl_t*)malloc(sizeof(gsp_view_impl_t));

    if (NULL == view) {
        return NULL;
    }

    view->previous_sibling = NULL;
    view->next_sibling = NULL;
    view->parent = NULL;
    view->child = NULL;
    view->window = NULL;
    
    if (NULL == root_view) {
        root_view = view;
    } else {

        gsp_view_impl_t* last_root = root_view;

        while (NULL != last_root->next_sibling) {
            last_root = last_root->next_sibling;
        }

        last_root->next_sibling = view;
        view->previous_sibling = last_root;
    }

    view->texture = gsp_renderer_allocate_texture();
    gsp_renderer_upload_file(view->texture, "sky.png");

    gsp_debug_log(INFO, "Allocated gview_t %lu", view);

    return (gview_t)view;
}

void gsp_view_destroy_view(gview_t view) {
    
   return;
}

void gsp_view_layout_window(gwindow_t window, gsize_t size) {
    gsp_view_impl_t* view = (gsp_view_impl_t*) gsp_view_get_window_root(window);

    if (view == NULL) {
        gsp_debug_log(INFO, "No root view set for gwindow_t %lu", window);
        return;
    }

    float vertices[] = {
        // Instance 1
        0.0, 0.0, size.width, size.height
    };

    uint32_t texture_ids[] = { 0u }; // Assuming texture 0 and texture 1 are used

    gsp_renderer_bind_texture(view->texture, 0);
    gsp_renderer_buffer_data(1, vertices, texture_ids);
    gsp_renderer_draw_instances(1);

    gsp_debug_log(INFO, "Laying out gwindow_t %lu to %f,%f", window, size.width, size.height);

}


bool gsp_view_is_view_valid(gview_t view) {

    if (NULL == root_view) {
        return false;
    }

    gsp_view_impl_t* current_view = root_view;

    while (NULL != current_view) {

        if (view == (gview_t)current_view) {
            return true;
        }

        if (current_view->child != NULL) {
            current_view = current_view->child;
        } else if (current_view->next_sibling != NULL) {
            current_view = current_view->next_sibling;
        } else {
            
            gsp_view_impl_t* parent = current_view->parent;

            while (parent != NULL && parent->next_sibling == NULL) {
                parent = parent->parent;
            }

            current_view = parent;
        }
    }

    return false;
}

bool gsp_view_is_view_root(gview_t view) {

    if (NULL == root_view) {
        return false;
    }

    gsp_view_impl_t* last_root = root_view;

    while (NULL != last_root) {

        if ((gview_t)last_root == view) {
            return true;
        }

        last_root = last_root->next_sibling;
    }

    return false;
    
}

gview_t gsp_view_get_window_root(gwindow_t window) {

    if (NULL == root_view) {
        printf("no root!\n");
        return NULL;
    }

    gsp_view_impl_t* last_root = root_view;

    while (NULL != last_root) {

        if (last_root->window == window) {
            return (gview_t)last_root;
        }

        last_root = last_root->next_sibling;
    }

    return NULL;
}

void gsp_view_set_window_root(gwindow_t window, gview_t view) {
    if (NULL == root_view) {
        return;
    } else if (!gsp_view_is_view_root(view)) {
        gsp_debug_log(WARNING, "Set can't set gview_t %lu as a child because it isn't a root view", view);
        return;
    }

    ((gsp_view_impl_t*)view)->window = window;

    gsp_debug_log(INFO, "Set gview_t %lu root for gwindow_t %lu", view, window);
}

void gsp_view_add_child(gview_t parent, gview_t child) {

    if (NULL == root_view) {
        return;
    } else if (!gsp_view_is_view_root(child)) {
        gsp_debug_log(WARNING, "Set can't set gview_t %lu as a child because it isn't a root view", child);
        return;
    } else if ( !gsp_view_is_view_valid(parent)) {
        gsp_debug_log(WARNING, "Invalid gview_t %lu", parent);
        return;
    }

    gsp_view_impl_t* child_impl = (gsp_view_impl_t*)child;
    gsp_view_impl_t* parent_impl = (gsp_view_impl_t*)parent;


    // remove view from tree

    if ((gview_t)root_view == child) {
        root_view = child_impl->next_sibling;
    } else if (child_impl->previous_sibling == NULL) {
        // i.e it was the first child
        child_impl->parent->child = child_impl->next_sibling;
    } else {
        // view wasn't root or the first child
        child_impl->previous_sibling->next_sibling = child_impl->next_sibling;
    }

    child_impl->next_sibling = NULL;
    child_impl->previous_sibling = NULL;


    // add view to tree
    if (parent_impl->child == NULL) {
        parent_impl->child = child_impl;
        child_impl->parent = parent_impl;
        child_impl->window = parent_impl->window;
    } else {
        gsp_view_impl_t* last_sibling = parent_impl->child;

        while (last_sibling->next_sibling != NULL) {
            last_sibling = last_sibling->next_sibling;
        }

        last_sibling->next_sibling = child_impl;
        child_impl->previous_sibling = last_sibling;
        child_impl->parent = parent_impl;
        child_impl->window = parent_impl->window;
    }

    gsp_debug_log(INFO, "Set gview_t %lu as a child of %lu", child, parent);
}

void gsp_view_children_count(gview_t parent, gview_t child) {

}

void gsp_view_set_background_color(gview_t view, gcolor_t color) {
    if (!gsp_view_is_view_valid(view)) {
        gsp_debug_log(WARNING, "Invalid gview_t %lu", view);
        return;
    }

    ((gsp_view_impl_t*)view)->background_color = color;
    gsp_debug_log(INFO, "Set background color for gview_t %lu", view);


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
