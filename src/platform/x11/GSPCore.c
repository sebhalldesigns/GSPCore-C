#include "GSPCore/GSPCore.h"
#include "internal/def/GApplicationDef.h"

#include "GSPCore/GLog.h"
#include "internal/def/GWindowDef.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int GSPRun(GApplication app) {

    GLog(INFO, "Starting GSPCore...");

    GApplicationDef* applicationDef = (GApplicationDef*)app; 


    if (app == 0 || applicationDef->launchEvent == 0) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (applicationDef->launchEvent)(applicationDef->userData);



    while (GWindowDef_NumberOfOpenWindows() > 0) {


        GWindowDef_Poll();
        //sleep(0);
    }

    GLog(INFO, "No windows open. Exiting application.");

    free(applicationDef);
    return 0;
}