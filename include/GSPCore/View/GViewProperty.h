#ifndef GVIEWPROPERTY_H
#define GVIEWPROPERTY_H

#include <GSPCore/Types/GSPCoreGraphics.h>
#include <GSPCore/Types/GSPCoreGeometry.h>

#include <stdlib.h>
#include <stddef.h>

typedef enum {
    PROPERTY_SELECTOR_NONE,    // Default  
    PROPERTY_SELECTOR_CHECK,   // Toggle state true
    PROPERTY_SELECTOR_DISABLE, // Enable = false
    PROPERTY_SELECTOR_FOCUS,   // Focused view
    PROPERTY_SELECTOR_HOVER,   // Cursor hovered over
    PROPERTY_SELECTOR_PRESS    // Being pressed/tapped 
} GViewPropertySelector;


typedef enum {
    PROPERTY_TYPE_NONE,
    PROPERTY_TYPE_BACKGROUND_COLOR,
    PROPERTY_TYPE_BORDER_COLOR,
    PROPERTY_TYPE_BORDER_THICKNESS, // shortcut for equal thicknesses
    PROPERTY_TYPE_BORDER_THICKNESSES,
    PROPERTY_TYPE_CORNER_RADIUS, // shortcut for equal radii
    PROPERTY_TYPE_CORNER_RADII,
    PROPERTY_TYPE_CURSOR_TYPE,
    // layout
    PROPERTY_TYPE_LAYOUT_TYPE,
    PROPERTY_TYPE_LAYOUT_ORIENTATION,
    PROPERTY_TYPE_LAYOUT_DIMENSIONS,
    PROPERTY_TYPE_MAX_SIZE,
    PROPERTY_TYPE_MIN_SIZE,
    PROPERTY_TYPE_OPACITY,
    PROPERTY_TYPE_OVERFLOW_TYPE_X,
    PROPERTY_TYPE_OVERFLOW_TYPE_Y,
    PROPERTY_TYPE_PADDING,
    PROPERTY_TYPE_REFERENCE_SIZE,
    PROPERTY_TYPE_RESIZE_HANDLE, // possibly make view a resize handle like WPF gridsplitter
    
    PROPERTY_TYPE_TEXT_ALIGN,
    PROPERTY_TYPE_TEXT,



} GViewPropertyType;

typedef struct {
    GColor color;
} GViewPropertyBackgroundColor;

typedef struct {
    GColor color;
} GViewPropertyBorderColor;

typedef struct {
    GFloat thickess;
} GViewPropertyBorderThickness;

typedef struct {
    GFloat leftThickness;
    GFloat topThickness;
    GFloat rightThickness;
    GFloat bottomThickness;
} GViewPropertyBorderThicknesses;

typedef struct {
    GFloat radius;
} GViewPropertyCornerRadius;

typedef struct {
    GFloat tlRadius;
    GFloat trRadius;
    GFloat brRadius;
    GFloat blRadius;
} GViewPropertyCornerRadii;

typedef enum {
    CURSOR_TYPE_DEFAULT,
    CURSOR_TYPE_HELP,
    CURSOR_TYPE_POINTER,
    CURSOR_TYPE_CELL,
    CURSOR_TYPE_CROSS,
    CURSOR_TYPE_TEXT,
    CURSOR_TYPE_VTEXT,
    CURSOR_TYPE_ALIAS,
    CURSOR_TYPE_COPY,
    CURSOR_TYPE_MOVE,
    CURSOR_TYPE_NOT_ALLOWED,
    CURSOR_TYPE_GRAB,
    CURSOR_TYPE_GRABBING,
    CURSOR_TYPE_RESIZE_COL,
    CURSOR_TYPE_RESIZE_ROW,
    CURSOR_TYPE_RESIZE_N,
    CURSOR_TYPE_RESIZE_S,
    CURSOR_TYPE_RESIZE_E,
    CURSOR_TYPE_RESIZE_W,
    CURSOR_TYPE_RESIZE_NS,
    CURSOR_TYPE_RESIZE_EW,
    CURSOR_TYPE_RESIZE_NE,
    CURSOR_TYPE_RESIZE_NW,
    CURSOR_TYPE_RESIZE_SE,
    CURSOR_TYPE_RESIZE_SW,
    CURSOR_TYPE_RESIZE_NESW,
    CURSOR_TYPE_RESIZE_NWSE,
    CURSOR_TYPE_ZOOM_IN,
    CURSOR_TYPE_ZOOM_OUT
} GViewPropertyCursorType;

typedef enum {
    LAYOUT_TYPE_NONE,
    LAYOUT_TYPE_DOCK,
    LAYOUT_TYPE_STACK,
    LAYOUT_TYPE_GRID,
    LAYOUT_TYPE_WRAP,
    LAYOUT_TYPE_PAGED
} GViewPropertyLayoutType;

typedef enum {
    LAYOUT_ORIENTATION_HORIZONTAL,
    LAYOUT_ORIENTATION_VERTICAL
} GViewPropertyLayoutOrientation;

typedef struct {
    size_t horizontal;
    size_t vertical;
} GViewPropertyLayoutDimensions;

typedef struct {
    bool fixWidth;
    bool fixHeight;
    GFloat width;
    GFloat height;
} GViewPropertyReferenceSize;

typedef struct {
    GFloat width;
    GFloat height;
} GViewPropertyMaxSize;

typedef struct {
    GFloat width;
    GFloat height;
} GViewPropertyMinSize;

typedef struct {
    GFloat opacity;
} GViewPropertyOpacity;

typedef enum {
    OVERFLOW_TYPE_CLIP,
    OVERFLOW_TYPE_VISIBLE,
    OVERFLOW_TYPE_SCROLL,
} GViewPropertyOverflowType;

typedef struct {
    GFloat left;
    GFloat top;
    GFloat right;
    GFloat bottom;
} GViewPropertyPadding;

typedef struct {
    GViewPropertySelector selector;
} GViewProperty;

#endif // GVIEWPROPERTY_H