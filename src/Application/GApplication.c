#include <GSPCore/GSPCore.h>
#include <GSPCore/Windowing/GWindowManager.h>
#include <GSPCore/Graphics/GRenderManager.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifdef GSPCORE_BUILD_UNIX
    const static GApplicationEnvironment environment = ENVIRONMENT_UNIX;
#endif

GWindow GApplication_GetMainWindow() {

}

void GApplication_SetMainWindow(GWindow window) {

}

GApplicationEnvironment GApplication_GetEnvironment() {
    return environment;
}


int GApplication_Run(GApplication *app) {

    if (app == NULL) {
        GLog(FAIL, "No app passed to run function.");
        return -1;
    }
    
    GLog(INFO, "Starting GSPCore");

    GWindowManager_Init();
    GRenderManager_Init();



    GApplicationController controller = app->applicationController;

    if (controller.launchEvent == NULL) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (controller.launchEvent)();

    

    return GWindowManager_RunLoop();

}
