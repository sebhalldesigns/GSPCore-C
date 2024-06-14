#include "GSPCore/GSPCore.h"
#include "GSPCore/Windowing/GWindowManager.h"
#include "GSPCore/Windowing/GWindow.h"


#include <stdio.h>

void launched() {

    printf("LAUNCHED!\n");

    GWindow window = GWindowManager_OpenWindow();
}

int main() {

    GApplication app = {
        .title = "example app", 
        .developer = "Seb Hall", 
        .majorVersion = 0, 
        .minorVersion = 0,
        .applicationController.launchEvent = launched
    };

    return GApplication_Run(&app);
}