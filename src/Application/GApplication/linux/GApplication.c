#include "GSPCore/Basic/GApplication.h"

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

    // Create an instance of UIApplication
    ((GApplicationDef*)app)->uiApplication = [UIApplication sharedApplication];
        
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
            [appDef->uiApplication setDelegate:controllerDef->uiKitApplicationDelegate];
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
        NSProcessInfo *processInfo = [NSProcessInfo processInfo];
        int argc = (int)[[processInfo arguments] count];
        char **argv = (char **)malloc(sizeof(char *) * (argc + 1));
        for (int i = 0; i < argc; i++) {
            NSString *arg = [[processInfo arguments] objectAtIndex:i];
            argv[i] = (char *)[arg UTF8String];
        }
        argv[argc] = NULL; // Set the last element to NULL as required by C conventions

        
        
        UIApplicationMain(argc, argv, nil, NSStringFromClass([UIKitApplicationDelegate class]));
        
        free(argv);
    }
    
    return 2;
}
