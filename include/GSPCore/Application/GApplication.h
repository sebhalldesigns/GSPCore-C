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


struct GApplication {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;

    // options only relevant to desktop platforms, ignored on portable platforms
    bool startSilently;
    bool exitOnRootWindowClose;
    int startWidth;
    int startHeight;

    GWindow* rootWindow;
};


#endif // GAPPLICATION_H