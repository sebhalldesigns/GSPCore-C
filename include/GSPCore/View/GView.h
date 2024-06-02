#ifndef GVIEW_H
#define GVIEW_H

// GView.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GView represents a generic UI element, containing
// rendering information and children.

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"
#include "../Types/GSPCoreGraphics.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>



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
    DOCK_FILL,
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

// MARK: FUNCTIONS

GView GView_Init(GViewInfo info);


// frees view and all children
void GView_Free(GView view);

// update frame of each child
void GView_LayoutChildren();

// render view and each child
void GView_Render(GView view);

void GView_AddSubview(GView view, GView subview);


//#ifdef GSPCORE_BUILD



typedef struct {
    GViewController controller;
    GRenderer renderer;

    char* name;
    size_t allocationIndex;

    GView parentView;
    GWindow parentWindow;

    GVector subviews;
    GViewLayout layout;
    GViewStackOrientation stackOrientation;
    GViewGridSize gridSize;

    GRect frame;
    GThickness margin;
    GThickness padding;
    GAlignment horizontalAlignment;
    GAlignment verticalAlignment;

    GColor backgroundColor;

} GViewDef;


//#endif

#endif // GVIEW_H