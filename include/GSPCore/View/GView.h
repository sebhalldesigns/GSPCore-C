#ifndef GVIEW_H
#define GVIEW_H

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <GSPCore/Types/GSPCoreGeometry.h>
#include <GSPCore/Types/GSPCoreGraphics.h>

#include <stddef.h>

// MARK: CONTAINER STRUCTS

typedef enum {
    LAYOUT_NONE,
    LAYOUT_DOCK,
    LAYOUT_STACK,
    //LAYOUT_GRID,
    //LAYOUT_WRAP,
    //LAYOUT_CAROUSEL
} GViewLayout;

typedef enum {
    STACK_VERTICAL,
    STACK_HORIZONTAL
} GViewStackOrientation;

typedef struct {
    size_t rows;
    size_t columns;
} GViewGridSize; 

// MARK: CHILD STRUCTS

typedef enum {
    DOCK_LEFT,
    DOCK_TOP,
    DOCK_RIGHT,
    DOCK_BOTTOM
} GViewDockLocation;

typedef struct {
    size_t row;
    size_t column;
} GViewGridLocation;

// MARK: GENERIC STRUCT

typedef struct {
    GRect frame;
    GColor backgroundColor;
    char* name;
} GViewInfo;

struct GView {
    GView* nextSibling;

    size_t numSubviews;
    GView* rootSubview;

    GViewLayout layout;
    GViewStackOrientation stackOrientation;
    GViewGridSize gridSize;

    GRect frame;
    GColor backgroundColor;
    GCornerRadii cornerRadii;

    GThickness margin;
    GThickness padding;
    GAlignment horizontalAlignment;
    GAlignment verticalAlignment;
    GViewDockLocation dockLocation;
};


#endif // GVIEW_H