#include "GSPCore/Controllers/GApplicationController.h"

#include <Windows.h>

GApplicationController GApplicationController_Init() {
    
    GApplicationControllerDef* controller = calloc(1, sizeof(GApplicationControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    return (GApplicationController)controller;
}

void GApplicationController_Free(GApplicationController applicationController) {
    free(applicationController);
}

void GApplicationController_SetLaunchEvent(GApplicationController applicationController, GApplicationLaunchEvent launchEvent) {
    if (applicationController == NULL) {
        return;
    }

    GApplicationControllerDef* controllerDef = (GApplicationControllerDef*)applicationController;
    controllerDef->launchEvent = launchEvent;
}

