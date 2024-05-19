#ifndef GDRAWCONTAINER_H
#define GDRAWCONTAINER_H

// GDrawContainer is a rendering object used to provide definition to child elements.

typedef enum {

    FILL, // most basic container, simply sets the size of each child to the full size of the container
    WRAP, 
    GRID,
    
} GDrawContainerType;


#endif // GDRAWCONTAINER_H