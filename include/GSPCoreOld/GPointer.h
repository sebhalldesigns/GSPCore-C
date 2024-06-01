#ifndef GPOINTER_H
#define GPOINTER_H

// GPointer is an implementation of a Reference-Counted smart pointer.

// A handle to the relevant free function for the object.
// All objects wrapped in a GPointer should contain a free function
// that takes 1 argument - the object pointer.
typedef void(*GPointerFreeFunction)(void* object);

typedef void* GPointer;


// allocates a GPointer for the object, returns new GPointer
GPointer GPointer_Alloc(void* object);

// get a raw handle to object from the GPointer. Increments reference count.
void* GPointer_Fetch(GPointer pointer);




#endif // GPOINTER_H