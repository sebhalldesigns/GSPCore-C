#ifndef GAPPLICATION_H
#define GAPPLICATION_H

// GApplication.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GApplication is the root application structure
// for working with the GSPCore system


#include "../Types/GSPCoreOpaqueTypes.h"

#include "../Controllers/GApplicationController.h"
#include <stdbool.h>

typedef enum {
    ENVIRONMENT_UNIX,
    ENVIRONMENT_MACOS,
    ENVIRONMENT_WIN32,
    ENVIRONMENT_IOS,
    ENVIRONMENT_ANDROID,
    ENVIRONMENT_WASM
} GApplicationEnvironment;

struct GApplication {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;

    // options only relevant to desktop platforms, ignored on portable platforms
    bool startSilently;
    int startWidth;
    int startHeight;

    GApplicationController applicationController;
};

// MARK: METHODS

GWindow GApplication_GetMainWindow();
void GApplication_SetMainWindow(GWindow window);
GApplicationEnvironment GApplication_GetEnvironment();


int GApplication_Run(GApplication *app);





#endif // GAPPLICATION_H