#ifndef GAPPLICATION_H
#define GAPPLICATION_H

typedef void* GApplication;

typedef struct {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;
} GApplicationInfo;

typedef void(*GApplicationLaunchEvent)();

// constructor requires info to create. Maybe info should only be constant for that object.
GApplication GApplication_Init(GApplicationInfo info);

// connect events after creations
void GApplication_SetLaunchEvent(GApplication app, GApplicationLaunchEvent launchEvent);


#endif // GAPPLICATION_H