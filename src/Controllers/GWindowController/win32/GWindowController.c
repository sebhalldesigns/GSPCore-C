#include "GSPCore/Controllers/GWindowController.h"

#include <stdlib.h>

GWindowController GWindowController_Init() {
    GWindowControllerDef* controller = calloc(1, sizeof(GWindowControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    return (GWindowController)controller;
}

void GWindowController_Free(GWindowController windowController) {
    free(windowController);
}

void GWindowController_SetResizeEvent(GWindowController windowController, GWindowResizeEvent resizeEvent) {
    if (windowController == NULL) {
        return;
    }

    GWindowControllerDef* controllerDef = (GWindowControllerDef*)windowController;
    controllerDef->resizeEvent = resizeEvent;
}

