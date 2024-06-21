#include "GSPCore/GSPCore.h"
#include "GSPCore/Window/GWindowManager.h"
#include "GSPCore/Window/GWindow.h"
#include "GSPCore/Resource/GResourceManager.h"

#include <stdio.h>

int main() {

    printf("STARTING...\n");

    if (!GSPCore_Init()) {
        return -1;
    }

    GRawImage image = GResourceManager_LoadImage("../resources/images/image.png");
    printf("image %lu %lu\n", image.width, image.height);

    image.width = 10;
    image.height = 10;

    GResourceManager_SaveImage(image, "image2.png");
    GApplication app;
    app.title = "MyApp";
    app.majorVersion = 0;
    app.minorVersion = 1;
    app.rootWindow = NULL;

    GWindow* window = GWindowManager_OpenWindow();
    app.rootWindow = window;
    printf("WINDOW IS %llu\n", window);

    return GApplicationManager_Run(&app);
}