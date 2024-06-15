#ifndef GWEBWINDOWMANAGER_H
#define GWEBWINDOWMANAGER_H

#include <stdbool.h>
#include <emscripten.h>

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>

typedef struct {
    int a;
} GWebState;

bool GWebWindowManager_TryInit();
GWindow* GWebWindowManager_OpenWindow();
int GWebWindowManager_RunLoop();

EMSCRIPTEN_KEEPALIVE
void GWebWindowManager_MouseMoveCallback(double mouseX, double mouseY);

EMSCRIPTEN_KEEPALIVE
void GWebWindowManager_ResizeCallback(double width, double height);

EMSCRIPTEN_KEEPALIVE
void GWebWindowManager_LightModeCallback();

EMSCRIPTEN_KEEPALIVE
void GWebWindowManager_DarkModeCallback();


#endif // GWEBWINDOWMANAGER_H