#include "GSPCore/GApplication.h"
#include "internal/def/GApplicationDef.h"

#include <stdlib.h>
#include <stddef.h>

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

void GApplication_SetUserData(GApplication app, void* userData) {
    if (app == NULL) {
        return;
    }

    ((GApplicationDef*)app)->userData = userData;
}

void GApplication_SetLaunchEvent(GApplication app, GApplicationLaunchEvent launchEvent) {
    if (app == NULL) {
        return;
    }

    ((GApplicationDef*)app)->launchEvent = launchEvent;
}