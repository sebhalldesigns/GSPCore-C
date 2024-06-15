#ifndef GAPPLICATIONCONTROLLER_H
#define GAPPLICATIONCONTROLLER_H

// MARK: PUBLIC INTERFACE

#include "../Types/GSPCoreOpaqueTypes.h"

typedef void (*GApplicationLaunchEvent)();
typedef void (*GApplicationColorModeChangeEvent)();

struct GApplicationController {
    GApplicationLaunchEvent launchEvent;
    GApplicationColorModeChangeEvent colorModeChanged;
};

#endif // GAPPLICATIONCONTROLLER_H