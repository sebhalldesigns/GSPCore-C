#ifndef GCOCOAWINDOWMANAGER_H
#define GCOCOAWINDOWMANAGER_H

#include <stdbool.h>

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface CocoaApplicationDelegate : NSObject <NSApplicationDelegate>
@end


typedef struct {
    CocoaApplicationDelegate* appDelegate;
} GCocoaState;

bool GCocoaWindowManager_TryInit();
GWindow* GCocoaWindowManager_OpenWindow();
int GCocoaWindowManager_RunLoop(GApplication* app);

#endif // GCOCOAWINDOWMANAGER_H