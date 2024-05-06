#ifndef GWINDOWDEF_H
#define GWINDOWDEF_H

#include "GSPCore/GWindow.h"

#include "GSPCore/Graphics/GGI.h"

#include <stdbool.h>

#ifdef _WIN32
    #include <Windows.h>
#endif


typedef struct {
    uintptr_t rawHandle;
    int width;
    int height;
    const char* title;
    GWindowDrawDelegate drawDelegate;
    GWindowCloseDelegate closeDelegate;
    GWindowWillCloseDelegate willCloseDelegate;
    GWindowGraphicsInterface graphicsInterface;
    GWindowResizeDelegate resizeDelegate;
    GWindowPointerMoveDelegate pointerMoveDelegate;
    GWindowButtonDownDelegate buttonDownDelegate;
    GWindowButtonUpDelegate buttonUpDelegate;

    #ifdef UNIX 
        uintptr_t glContext;
        uintptr_t xDeleteAtom;
    #endif

    #ifdef _WIN32
        HGLRC glContext;
    #endif

    GShader shader;
    GTexture texture;
    GVertexBuffer vertexBuffer;
} GWindowDef;

// called once by system
bool GWindowDef_Init();
void GWindowDef_Poll();
size_t GWindowDef_NumberOfOpenWindows();

#endif // GWINDOWDEF_H