#ifndef GAPPLICATIONDEF_H
#define GAPPLICATIONDEF_H

#include "GSPCore/GApplication.h"

typedef struct {
    void* userData;
    GApplicationInfo info;
    GApplicationLaunchEvent launchEvent;
} GApplicationDef;

static GApplicationDef* app = 0;

#endif // GAPPLICATIONDEF_H