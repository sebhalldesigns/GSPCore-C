#include <GSPCore/GSPCore.h>
#include <GSPCore/Application/GApplicationManager.h>
#include <GSPCore/Application/GApplication.h>
#include <GSPCore/Window/GWindowManager.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifdef GSPCORE_BUILD_UNIX
    const static GApplicationEnvironment environment = ENVIRONMENT_UNIX;
#elif GSPCORE_BUILD_WIN32
    const static GApplicationEnvironment environment = ENVIRONMENT_WIN32;
#elif GSPCORE_BUILD_MACOS
    const static GApplicationEnvironment environment = ENVIRONMENT_MACOS;
#elif GSPCORE_BUILD_WEB
    const static GApplicationEnvironment environment = ENVIRONMENT_WEB;
#endif



bool GApplicationManager_Init() {
    return true;
}

int GApplicationManager_Run(GApplication* app) {
    
    GLog(INFO, "Started main loop");
    
    while (app->rootWindow != NULL) {

        if (GWindowManager_RunLoop(app) < 0) {
           GLog(FAIL, "A window manager error occured! Exiting...");
           return -1;
        }
    }

    GLog(INFO, "No windows open! Exiting...");
    return 0;
}

GApplicationEnvironment GApplicationManager_GetEnvironment() {
    return environment;
}
