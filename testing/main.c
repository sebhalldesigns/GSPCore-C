#include "GSPCore/GSPCore.h"
#include "GSPCore/GLog.h"

#include <stdio.h>

void LaunchEvent() {
    printf("Launched!\n");
    /*GWindowInfo info = {
        800,
        600,
        "GSP Test Window"
    };

    GWindow window = GWindow_Init(info);
    GWindow_SetResizeDelegate(window, WindowResized);
    GWindow_SetPointerMoveDelegate(window, MouseMove);
    GWindow_SetButtonDownDelegate(window, MouseDown);
    GWindow_SetButtonUpDelegate(window, MouseUp);
    GWindow_SetDrawDelegate(window, Draw);*/

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