#include <GSPCore/GSPCore.h>
#include <GSPCore/Application/GApplication.h>
#include <GSPCore/Window/Platforms/Cocoa/GCocoaWindowManager.h>
#include <GSPCore/Window/GWindow.h>

#include <GSPCore/Render/GRenderManager.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static GCocoaState state;
static GWindow* rootWindow = NULL;

@interface CocoaWindow : NSWindow
@property GWindow* window;
@end

@implementation CocoaWindow

@end

bool GCocoaWindowManager_TryInit() {

    GLog(INFO, "Started GWindowManager on Cocoa backend");

    return true;
}

GWindow* GCocoaWindowManager_OpenWindow() {

    GWindow* window = calloc(1, sizeof(GWindow));

    printf("ALLOCATED WINDOW %llu\n", window);

    @autoreleasepool {
        NSRect frame = NSMakeRect(0, 0, 800, 600);
        window->platformHandles.window = [[CocoaWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled |
                                                         NSWindowStyleMaskClosable |
                                                         NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
        printf("INIT WINDOW 2\n");

        //window->cocoaWindow.window = window;

        //NSString* titleString = [NSString stringWithUTF8String:info.title];
        [window->platformHandles.window setTitle:@"example window"];

        [window->platformHandles.window center];
        [window->platformHandles.window makeKeyAndOrderFront:nil];     
        
        

    }


    return window;
}

int GCocoaWindowManager_RunLoop(GApplication* app) {
    printf("cocoa manager run loop\n");

    @autoreleasepool {
        NSApplication* app = [NSApplication sharedApplication];
        [app run];
    }

    return 0;
}

@implementation CocoaWindowDelegate

- (void)windowDidResize:(NSNotification *)notification {

   printf("resize\n");
}

@end