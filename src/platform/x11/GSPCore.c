#include "GSPCore/GSPCore.h"
#include "internal/def/GApplicationDef.h"

#include "GSPCore/GLog.h"
#include "GSPCore/GRenderManager.h"
#include "GSPCore/GHiperRenderManager.h"
#include "internal/def/GWindowDef.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int GSPRun(GApplication app) {

    GLog(INFO, "Starting GSPCore...");


    GRenderManager_Setup(DEFAULT);

    GApplicationDef* applicationDef = (GApplicationDef*)app; 

    if (app == 0 || applicationDef->launchEvent == 0) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (applicationDef->launchEvent)(applicationDef->userData);



    while (GWindowDef_NumberOfOpenWindows() > 0) {


        GWindowDef_Poll();
        usleep(10);
    }

    GLog(INFO, "No windows open. Exiting application.");

    GRenderManager_Cleanup();

    free(applicationDef);
    return 0;
}