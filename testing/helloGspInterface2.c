#include "GSPCore2/GSPCore.h"

#include <stdio.h>

void WindowResized(GWindow window, GSize size) {
    printf("WINDOW RESIZED: %f %f\n", size.width, size.height);
}

void AppLaunched() {

    printf("App launched!\n");
    
    GWindowInfo info;
    info.title = "MyApp Window";
    info.size.width = 800;
    info.size.height = 600;

    GWindow window = GWindow_Init(info);

    GWindowController windowController = GWindowController_Init();
    GWindowController_SetResizeEvent(windowController, WindowResized);

    GWindow_SetController(window, windowController);

    printf("FUNCTION COMPLETE\n");
}

int main() {

    printf("HELLO WORLD\n");    

    GApplicationInfo info;
    info.title = "MyApp";
    info.developer = "Seb Hall";
    info.majorVersion = 0;
    info.minorVersion = 1;

    GApplication app = GApplication_Init(info);

    GApplicationController appController = GApplicationController_Init();
    GApplicationController_SetLaunchEvent(appController, AppLaunched);

    GApplication_SetController(app, appController);

    return GApplication_Run(app);
}