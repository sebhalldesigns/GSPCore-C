#ifndef GAPPLICATIONCONTROLLER_H
#define GAPPLICATIONCONTROLLER_H

// MARK: PUBLIC INTERFACE

#include "../Types/GSPCoreOpaqueTypes.h"

typedef void (*GApplicationLaunchEvent)(GApplication application);

GApplicationController GApplicationController_Init();
void GApplicationController_Free(GApplicationController applicationController);

void GApplicationController_SetLaunchEvent(GApplicationController applicationController, GApplicationLaunchEvent launchEvent);

#ifdef GSPCORE_BUILD_WASM

typedef struct {
    GApplicationLaunchEvent launchEvent;
} GApplicationControllerDef;

#endif

#ifdef GSPCORE_BUILD_IOS

#import <UIKit/UIKit.h>

@interface UIKitApplicationDelegate : UIResponder <UIApplicationDelegate>
@property GApplicationController controller;
@end

typedef struct {
    UIKitApplicationDelegate* uiKitApplicationDelegate;
    GApplicationLaunchEvent launchEvent;
} GApplicationControllerDef;

#endif

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

#ifdef GSPCORE_BUILD_WIN32

typedef struct {
    GApplicationLaunchEvent launchEvent;
} GApplicationControllerDef;

#endif 



#endif // GAPPLICATIONCONTROLLER_H