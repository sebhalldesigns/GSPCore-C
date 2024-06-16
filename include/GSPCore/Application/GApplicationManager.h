#ifndef GAPPLICATIONMANAGER_H
#define GAPPLICATIONMANAGER_H


#include <stdbool.h>
#include <GSPCore/Types/GSPCoreOpaqueTypes.h>

typedef enum {
    ENVIRONMENT_UNIX,
    ENVIRONMENT_MACOS,
    ENVIRONMENT_WIN32,
    ENVIRONMENT_IOS,
    ENVIRONMENT_ANDROID,
    ENVIRONMENT_WEB
} GApplicationEnvironment;


bool GApplicationManager_Init();

int GApplicationManager_Run(GApplication* app);

GApplicationEnvironment GApplicationManager_GetEnvironment();


#endif // GAPPLICATIONMANAGER_H