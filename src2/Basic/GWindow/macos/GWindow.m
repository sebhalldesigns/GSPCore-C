#include "GSPCore2/Basic/GWindow.h"

GWindow GWindow_Init(GWindowInfo info) {


    GWindowDef* window = calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    window->info = info;

    @autoreleasepool {
        NSRect frame = NSMakeRect(0, 0, info.size.width, info.size.height);
        window->cocoaWindow = [[CocoaWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled |
                                                         NSWindowStyleMaskClosable |
                                                         NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];

        NSString* titleString = [NSString stringWithUTF8String:info.title];
        [window->cocoaWindow setTitle:titleString];
        [window->cocoaWindow center];
        [window->cocoaWindow makeKeyAndOrderFront:nil];     
        window->cocoaWindow.window = window;
    }

    return window;
}

void GWindow_Free(GWindow window) {
    free(window);
}

void GWindow_SetController(GWindow window, GWindowController controller) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->controller = controller;

    @autoreleasepool {
        if (controller != NULL) {
            GWindowControllerDef* controllerDef = (GWindowControllerDef*)controller;
            [windowDef->cocoaWindow setDelegate:controllerDef->cocoaWindowDelegate];
        }
    }
}

@implementation CocoaWindow

@end