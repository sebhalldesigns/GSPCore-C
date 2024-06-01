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

    return app;
}

void GApplication_SetController(GApplication application, GApplicationController controller) {
    if (application == NULL) {
        return;
    }

    ((GApplicationDef*)application)->controller = controller;
}

int GApplication_Run(GApplication application) {

    printf("RUNNING\n");
    if (application == NULL) {
        return -1;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;

    @autoreleasepool {
        appDef->nsApplication = [NSApplication sharedApplication];

        if (appDef->controller != NULL) {
            GApplicationControllerDef* controllerDef = (GApplicationControllerDef*)appDef->controller;
            [appDef->nsApplication setDelegate:controllerDef->cocoaApplicationDelegate];
        } else {
            printf("NO DELEGATE SET\n");
        }

        [appDef->nsApplication run];
    }
}