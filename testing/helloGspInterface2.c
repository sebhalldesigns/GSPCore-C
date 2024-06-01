#include "GSPCore2/GSPCore.h"

#include <stdio.h>

void AppLaunched() {
    printf("App launched!\n");
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
    GApplicationController_SetLaunchedEvent(appController, AppLaunched);

    GApplication_SetController(app, appController);

    return GApplication_Run(app);
}