#include "GSPCore2/Controllers/GApplicationController.h"

GApplicationController GApplicationController_Init() {
    GApplicationControllerDef* controller = calloc(1, sizeof(GApplicationControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    @autoreleasepool {
        controller->cocoaApplicationDelegate = [[CocoaApplicationDelegate alloc] init];
    }

    controller->cocoaApplicationDelegate.controller = controller;

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


@implementation CocoaApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {

    GApplicationControllerDef* controllerDef = (GApplicationControllerDef*)self.controller;

    if (controllerDef->launchEvent != NULL) {
        (controllerDef->launchEvent)();
    }

}

@end

