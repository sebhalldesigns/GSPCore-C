#ifndef GVIEWDEF_H
#define GVIEWDEF_H

#include "GSPCore/UI/GView.h"

#ifdef EMSCRIPTEN
    #include <GLES3/gl3.h>
#endif

typedef struct {
    GRect frame;
    GColor color;

    #ifdef EMSCRIPTEN
        GLuint framebuffer;
        GLuint texture;
    #endif
    
} GViewDef;


#endif // GVIEWDEF_H