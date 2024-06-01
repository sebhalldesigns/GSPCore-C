#include "GSPCore2/Controllers/GApplicationController.h"

#include <stdio.h>

GApplicationController GApplicationController_Init() {
    GApplicationControllerDef* controller = calloc(1, sizeof(GApplicationControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    controller->uiKitApplicationDelegate = NULL;

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


@implementation UIKitApplicationDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    printf("APPLICATION DID FINISH LAUNCHING!\n");

    // this is a bit different to on macOS becuase the UIApplicationDelegate must be instantiated by the system.
    if (app == NULL) {
        return false;
    } else {
        self.controller = ((GApplicationDef*)app)->controller;
    }

    GApplicationControllerDef* controllerDef = (GApplicationControllerDef*)self.controller;

    if (controllerDef->launchEvent != NULL) {
        (controllerDef->launchEvent)();
    }
    return true;
}


@end


