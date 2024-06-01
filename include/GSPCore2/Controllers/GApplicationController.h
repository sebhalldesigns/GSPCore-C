#ifndef GAPPLICATIONCONTROLLER_H
#define GAPPLICATIONCONTROLLER_H

// MARK: PUBLIC INTERFACE

#include "../Basic/GApplication.h"

typedef void* GApplicationController;

typedef void (*GApplicationLaunchEvent)();

GApplicationController GApplicationController_Init();
void GApplicationController_Free(GApplicationController applicationController);

void GApplicationController_SetLaunchEvent(GApplicationController applicationController, GApplicationLaunchEvent launchEvent);


#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

@interface CocoaApplicationDelegate : NSObject <NSApplicationDelegate>
@property GApplicationController controller;
@end


typedef struct {
    CocoaApplicationDelegate* cocoaApplicationDelegate;
    GApplicationLaunchEvent launchEvent;
} GApplicationControllerDef;

#endif



#endif // GAPPLICATIONCONTROLLER_H