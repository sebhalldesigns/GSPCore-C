#ifndef GTEXTPROVIDERDEF_H
#define GTEXTPROVIDERDEF_H

#ifdef UNIX
    #include <cairo.h>
#endif

typedef struct {
    char* typeface;
    float size;
    
    #ifdef UNIX
        cairo_surface_t* measureSurface;
        cairo_t* measureContext; 
    #endif

} GTextProviderDef;


#endif // GTEXTPROVIDERDEF_H