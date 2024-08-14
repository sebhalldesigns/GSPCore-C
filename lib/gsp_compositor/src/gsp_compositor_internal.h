#ifndef GSP_COMPOSITOR_INTERNAL_H
#define GSP_COMPOSITOR_INTERNAL_H

#include <gsp_compositor/gsp_compositor.h>
#include <gsp_types/gsp_types.h>

typedef enum {
    NONE,
    STACK,
    DOCK,
    GRID,
    WRAP
} gsurface_layout_t;

typedef enum {
    HORIZONTAL,
    VERTICAL
} gsurface_direction_t;


typedef enum {
    HORIZONTAL,
    VERTICAL
} gsurface_dock_t;

typedef union {
    
    struct {
        gsurface_direction_t direction;
    } stack_info;
    
    struct {
        size_t grid_rows;
        size_t grid_columns;
    } grid_info;

    struct {
        gsurface_direction_t direction;
    } wrap_info;
    
} gsurface_parent_layout_info_t;

typedef union {
    
    struct {
        gsurface_dock_t dock_request;
    } dock_info;
    
    struct {
        size_t row_request;
        size_t column_request;
    } grid_info;

} gsurface_child_layout_info_t;

typedef struct {
    gcolor_t background;
    gsize_t size_request;

    gsurface_layout_t layout;
    gsurface_parent_layout_info_t parent_layout_info;

    gsurface_child_layout_info_t child_layout_info;

} gsurface_contents_t;

#endif // GSP_COMPOSITOR_INTERNAL_H

