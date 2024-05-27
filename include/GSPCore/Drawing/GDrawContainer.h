#ifndef GDRAWCONTAINER_H
#define GDRAWCONTAINER_H

// GDrawContainer is a rendering object used to provide definition to child elements.

typedef enum {

    FILL, // most basic container, simply sets the size of each child to the full size of the container
    LAYOUT_WRAP, // a wrap render type, designed specifically for text rendering
    DOCK,

} GDrawContainerLayoutType;


#endif // GDRAWCONTAINER_H