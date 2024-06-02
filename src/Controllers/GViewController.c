#include "GSPCore/Controllers/GViewController.h"
#include "GSPCore/GSPCore.h"

GViewController GViewController_Init() {
    GViewControllerDef* controller = calloc(1, sizeof(GViewControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    return (GViewController)controller;
}

void GViewController_Free(GViewController viewController) {
    free(viewController);
}


void GViewController_SetLoadedEvent(GViewController viewController, GViewLoadedEvent loadedEvent) {
    if (viewController == NULL) {
        return;
    }

    GViewControllerDef* controllerDef = (GViewControllerDef*)viewController;
    controllerDef->loadedEvent = loadedEvent;
}

void GViewController_SetMouseEnterEvent(GViewController viewController, GViewMouseEnterEvent mouseEnterEvent) {
    if (viewController == NULL) {
        return;
    }

    GViewControllerDef* controllerDef = (GViewControllerDef*)viewController;
    controllerDef->mouseEnterEvent = mouseEnterEvent;
}

void GViewController_SetMouseExitEvent(GViewController viewController, GViewMouseExitEvent mouseExitEvent) {
    if (viewController == NULL) {
        return;
    }

    GViewControllerDef* controllerDef = (GViewControllerDef*)viewController;
    controllerDef->mouseExitEvent = mouseExitEvent;
}