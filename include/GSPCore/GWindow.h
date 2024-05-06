#ifndef GWINDOW_H
#define GWINDOW_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Graphics/GFrame.h"

typedef void* GWindow;

typedef struct {
    int width;
    int height;
    const char* title;
} GWindowInfo;

typedef struct {
    int width;
    int height;
} GWindowSize;

typedef struct {
    int x;
    int y;
} GWindowPoint;

typedef enum {
    NONE,
    SGI,
    OPENGL,
    WEBGL,
    METAL
} GWindowGraphicsInterface;

typedef void(*GWindowDrawDelegate)(GWindow window, GFrame surface);

typedef void(*GWindowCloseDelegate)(GWindow window);
typedef bool(*GWindowWillCloseDelegate)(GWindow window);

typedef void(*GWindowResizeDelegate)(GWindow window, GWindowSize size);
typedef void(*GWindowPointerMoveDelegate)(GWindow window, GWindowPoint newLocation);
typedef void(*GWindowButtonDownDelegate)(GWindow window, GWindowPoint location, uint8_t button);
typedef void(*GWindowButtonUpDelegate)(GWindow window, GWindowPoint location, uint8_t button);

GWindow GWindow_Init(GWindowInfo info);
void GWindow_Close(GWindow window);
void GWindow_PresentFrame(GFrame frame);

GWindowGraphicsInterface GWindow_GetGraphicsInterface(GWindow window);

// connect delegates after creation
void GWindow_SetDrawDelegate(GWindow window, GWindowDrawDelegate drawDelegate);
void GWindow_SetCloseDelegate(GWindow window, GWindowCloseDelegate closeDelegate);
void GWindow_SetWillCloseDelegate(GWindow window, GWindowWillCloseDelegate willCloseDelegate);
void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate);
void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate);
void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate);
void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate);


#endif // GWINDOW_H