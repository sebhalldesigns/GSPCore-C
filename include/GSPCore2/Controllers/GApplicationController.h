#ifndef GAPPLICATIONCONTROLLER_H
#define GAPPLICATIONCONTROLLER_H

// MARK: PUBLIC INTERFACE

#include "../Basic/GApplication.h"

typedef void* GApplicationController;

typedef void (*GApplicationLaunchedEvent)();

GApplicationController GApplicationController_Init();
void GApplicationController_Free(GApplicationController applicationController);

void GApplicationController_SetLaunchedEvent(GApplicationController applicationController, GApplicationLaunchedEvent launchedEvent);


#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

@interface CocoaApplicationDelegate : NSObject <NSApplicationDelegate>
@property GApplicationController controller;
@end


typedef struct {
    CocoaApplicationDelegate* cocoaApplicationDelegate;
    GApplicationLaunchedEvent launchedEvent;
} GApplicationControllerDef;

#endif



#endif // GAPPLICATIONCONTROLLER_H