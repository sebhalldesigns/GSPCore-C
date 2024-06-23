#ifndef GVIEWSTATE_H
#define GVIEWSTATE_H

#include <GSPCore/View/GViewProperty.h>

// GViewState is a structure containing the calculated value of each property.

typedef struct {

    GViewPropertyBackgroundColor backgroundColor;
    GViewPropertyBorderColor borderColor;
    GViewPropertyBorderThicknesses borderThicknesses;
    GViewPropertyCornerRadii cornerRadii;
    GViewPropertyCursorType cursorType;

    GViewPropertyLayoutType layoutType;
    GViewPropertyLayoutOrientation layoutOrientation;
    GViewPropertyLayoutDimensions layoutDimensions;

    GViewPropertyMaxSize maxSize;
    GViewPropertyMaxSize minSize;

    GViewPropertyOpacity opacity;
    GViewPropertyOverflowType overflowTypeX;
    GViewPropertyOverflowType overflowTypeY;

    GViewPropertyPadding padding;

    GViewPropertyReferenceSize referenceSize;

} GViewState;


const int a = sizeof(GViewState);


#endif // GVIEWSTATE_H