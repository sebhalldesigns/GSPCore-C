#ifndef GTEXTPROVIDERDEF_H
#define GTEXTPROVIDERDEF_H

#ifdef __unix__
    #include <cairo.h>
#endif

typedef struct {
    char* typeface;
    float size;
    
    #ifdef __unix__
        cairo_surface_t* measureSurface;
        cairo_t* measureContext; 
    #endif

} GTextProviderDef;


#endif // GTEXTPROVIDERDEF_H