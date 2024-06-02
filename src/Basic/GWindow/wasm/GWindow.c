#include "GSPCore/Basic/GWindow.h"
#include "GSPCore/GSPCore.h"

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


void GWindow_Render(GWindow window) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    
    if (windowDef->rootView == NULL) {
        GLog(WARNING, "GWindow has no root view!");
        return;
    }

    GViewDef* viewDef = (GViewDef*)windowDef->rootView;
    GRenderer_RenderRoot(viewDef->renderer);
}

void GWindow_Layout(GWindow window) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    
    if (windowDef->rootView == NULL) {
        GLog(WARNING, "GWindow has no root view!");
        return;
    }

    GViewDef* viewDef = (GViewDef*)windowDef->rootView;
    viewDef->frame.origin = (GPoint) { 0.0, 0.0 }; 
    viewDef->frame.size = (GSize) {windowDef->info.size.width, windowDef->info.size.height };
    
}

void GWindow_SetController(GWindow window, GWindowController controller) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->controller = controller;

}

GView GWindow_GetRootView(GWindow window) {
    if (window == NULL) {
        return NULL;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    return windowDef->rootView;
}

void GWindow_SetRootView(GWindow window, GView view) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->rootView = view;
}