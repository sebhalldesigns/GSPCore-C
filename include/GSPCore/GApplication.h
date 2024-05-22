#ifndef GAPPLICATION_H
#define GAPPLICATION_H

#include <stdbool.h>

typedef void* GApplication;

typedef struct {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;

    // options only relevant to desktop platforms, ignored on portable platforms
    bool startSilently;
    int startWidth;
    int startHeight;
} GApplicationInfo;

typedef void (*GApplicationLaunchEvent)();

// constructor requires info to create. Maybe info should only be constant for that object.
GApplication GApplication_Init(GApplicationInfo info);
void GApplication_SetUserData(GApplication app, void* userData);

// connect events after creations
void GApplication_SetLaunchEvent(GApplication app, GApplicationLaunchEvent launchEvent);


#endif // GAPPLICATION_H