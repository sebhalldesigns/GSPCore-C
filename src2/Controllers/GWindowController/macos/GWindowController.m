#include "GSPCore2/Controllers/GWindowController.h"

GWindowController GWindowController_Init() {
    GWindowControllerDef* controller = calloc(1, sizeof(GWindowControllerDef));

    if (controller == NULL) {
        return NULL;
    }

    @autoreleasepool {
        controller->cocoaWindowDelegate = [[CocoaWindowDelegate alloc] init];
    }

    controller->cocoaWindowDelegate.controller = controller;

    return (GWindowController)controller;
}

void GWindowController_Free(GWindowController windowController) {
    free(windowController);
}

void GWindowController_SetResizeEvent(GWindowController windowController, GWindowResizeEvent resizeEvent) {
    if (windowController == NULL) {
        return;
    }

    GWindowControllerDef* controllerDef = (GWindowControllerDef*)windowController;
    controllerDef->resizeEvent = resizeEvent;
}


@implementation CocoaWindowDelegate

- (void)windowDidResize:(NSNotification *)notification {

    CocoaWindow* window = [notification object];
    NSRect frame = [window frame];

    GWindowControllerDef* controllerDef = (GWindowControllerDef*)self.controller;
    GWindowDef* windowDef = (GWindowDef*)window.window;
    windowDef->info.size.width = frame.size.width;
    windowDef->info.size.height = frame.size.height;

    if (controllerDef->resizeEvent != NULL) {
        (controllerDef->resizeEvent)(window.window, windowDef->info.size);
    }
}


@end

