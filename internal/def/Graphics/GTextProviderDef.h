#ifndef GTEXTPROVIDERDEF_H
#define GTEXTPROVIDERDEF_H

#include <cairo.h>

typedef struct {
    char* typeface;
    float size;
    cairo_surface_t* measureSurface;
    cairo_t* measureContext; 
} GTextProviderDef;


#endif // GTEXTPROVIDERDEF_H