#include "GSPCore2/Basic/GApplication.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

GApplication GApplication_Init(GApplicationInfo info) {

    // check if there is already an app
    if (app != NULL) {
        return NULL;
    }

    app = calloc(1, sizeof(GApplicationDef));

    if (app == NULL) {
        return NULL;
    }

    ((GApplicationDef*)app)->info = info;

    @autoreleasepool {
        ((GApplicationDef*)app)->nsApplication = [NSApplication sharedApplication];
    }

    return app;
}

void GApplication_Free(GApplication application) {
    free(application);
}

void GApplication_SetController(GApplication application, GApplicationController controller) {
    if (application == NULL) {
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;

    appDef->controller = controller;

    @autoreleasepool {
        if (controller != NULL) {
            GApplicationControllerDef* controllerDef = (GApplicationControllerDef*)controller;
            [appDef->nsApplication setDelegate:controllerDef->cocoaApplicationDelegate];
        }
    }
}

int GApplication_Run(GApplication application) {

    printf("RUNNING\n");
    if (application == NULL) {
        return -1;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;

    @autoreleasepool {
        [appDef->nsApplication run];
    }
}