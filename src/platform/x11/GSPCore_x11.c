#include "GSPCore/GSPCore.h"
#include "internal/def/GApplicationDef.h"

#include "GSPCore/GLog.h"
#include "internal/def/GWindowDef.h"

#include <unistd.h>

int GSPRun(GApplication app) {

    GLog(INFO, "Starting GSPCore...");


    if (app == 0 || ((GApplicationDef*)app)->launchEvent == 0) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (((GApplicationDef*)app)->launchEvent)();

    while (GWindowDef_NumberOfOpenWindows() > 0) {
        GWindowDef_Poll();
        sleep(0);
    }

    return 0;
}