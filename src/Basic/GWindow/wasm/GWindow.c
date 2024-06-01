#include "GSPCore/Basic/GWindow.h"

#include "GSPCore/Basic/GApplication.h"
#include "GSPCore/Basic/GVector.h"

#include "GSPCore/Basic/GLog.h"

GWindow GWindow_Init(GWindowInfo info) {

    GWindowDef* window = calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }


    window->info = info;

    return window;
}


void GWindow_Free(GWindow window) {
    
    if (window == NULL) {
        return;
    }

    free(window);
}


void GWindow_Close(GWindow window) {

    GLog(WARNING, "GWindow Close called in WASM! Ignoring!");

    return;
}


void GWindow_SetController(GWindow window, GWindowController controller) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->controller = controller;

}