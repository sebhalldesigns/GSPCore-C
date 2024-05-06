#include "GSPCore/GSPCore.h"
#include "GSPCore/GWindow.h"
#include "GSPCore/GLog.h"

#include "GSPCore/Graphics/GTextProvider.h"

#include <stdio.h>


void Draw(GWindow window) {
    printf("DRAW\n");
}

void WindowResized(GWindow window, GWindowSize newSize) {
    printf("window resized to %d %d \n", newSize.width, newSize.height);

    GTextProvider textProvider = GTextProvider_Alloc("Ubuntu Mono", 20.0);

    GTexture texture = GTextProvider_Render(textProvider, "hello world from GSPCore!");

    GTextProvider_Free(textProvider);
}

void MouseMove(GWindow window, GWindowPoint point) {
    printf("mouse moved to %d %d\n", point.x, point.y);
}

void MouseDown(GWindow window, GWindowPoint point, uint8_t button) {
    printf("mouse %u down\n", button);
}

void MouseUp(GWindow window, GWindowPoint point, uint8_t button) {
    printf("mouse %u up\n", button);
}

void LaunchEvent() {
    printf("Launched!\n");
    GWindowInfo info = {
        800,
        600,
        "GSP Test Window"
    };

    GWindow window = GWindow_Init(info);
    GWindow_SetResizeDelegate(window, WindowResized);
    GWindow_SetPointerMoveDelegate(window, MouseMove);
    GWindow_SetButtonDownDelegate(window, MouseDown);
    GWindow_SetButtonUpDelegate(window, MouseUp);
    GWindow_SetDrawDelegate(window, Draw);

}

int main() {

    GApplicationInfo info = {
        "example app", 
        "Seb Hall", 
        0, 0
    };

    GApplication app = GApplication_Init(info);
    
    GApplication_SetLaunchEvent(app, LaunchEvent);

    return GSPRun(app);
}