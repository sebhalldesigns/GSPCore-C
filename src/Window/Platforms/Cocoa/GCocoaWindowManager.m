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
                                                         NSWindowStyleMaskMiniaturizable |
                                                         NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
        printf("INIT WINDOW 2\n");

        //window->cocoaWindow.window = window;

        //NSString* titleString = [NSString stringWithUTF8String:info.title];
        [window->platformHandles.window setTitle:@"example window"];

        window->platformHandles.metalView = [[CocoaMetalView alloc] initWithFrame:frame device:MTLCreateSystemDefaultDevice() window:window];
        
        [window->platformHandles.window setContentView:window->platformHandles.metalView];

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

@implementation CocoaWindow

@end

@implementation CocoaMetalView

- (instancetype)initWithFrame:(NSRect)frameRect device:(id<MTLDevice>)device window:(GWindow*)gwindow {
    self = [super initWithFrame:frameRect device:device];

    if (self) {
        // Configure the Metal view
        self.clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0); // Clear color is light gray
        self.paused = true;
        self.enableSetNeedsDisplay = true;
        self.commandQueue = [self.device newCommandQueue];
        printf("WINDOW IS %llu\n", window);
        window = gwindow;    
        printf("WINDOW IS %llu\n", window);
    }

    printf("MADE\n");

    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    GRenderManager_RenderWindow(window);
}

@end