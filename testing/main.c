#include "GSPCore/GSPCore.h"
#include "GSPCore/Window/GWindowManager.h"
#include "GSPCore/Window/GWindow.h"
#include "GSPCore/Resource/GResourceManager.h"
#include "GSPCore/View/GViewManager.h"

#include "GSPCore/Markup/GMarkupManager.h"

#include <stdio.h>

int main() {

    printf("STARTING...\n");


    GMarkupManager_Load("../resources/gsml/example.gsml");
    return -1;

    if (!GSPCore_Init()) {
        return -1;
    }

    GRawImage image = GResourceManager_LoadImage("../resources/images/image.png");
    printf("image %lu %lu\n", image.width, image.height);


    GResourceManager_SaveImage(image, "image2.png");
    GApplication app;
    app.title = "MyApp";
    app.majorVersion = 0;
    app.minorVersion = 1;
    app.rootWindow = NULL;

    GSize size = GResourceManager_MeasureText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur ut iaculis ex. Morbi eu posuere odio. Sed aliquet hendrerit augue, ac fringilla elit consectetur eget. Phasellus nec molestie sem, a vehicula neque. Maecenas pretium fringilla massa vel tincidunt. Phasellus id urna diam. Duis sollicitudin lacinia mauris, vitae facilisis sapien porta vel. Proin elementum augue at auctor congue. Nulla dapibus hendrerit eros vitae porta. Aliquam sit amet nisi ex. In cursus finibus orci, quis dapibus ex varius vitae. Aliquam erat volutpat. Nullam in maximus nunc. Curabitur at varius eros. ", "JetBrains Mono", 20);

    GRawImage textImage = GResourceManager_LoadText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur ut iaculis ex. Morbi eu posuere odio. Sed aliquet hendrerit augue, ac fringilla elit consectetur eget. Phasellus nec molestie sem, a vehicula neque. Maecenas pretium fringilla massa vel tincidunt. Phasellus id urna diam. Duis sollicitudin lacinia mauris, vitae facilisis sapien porta vel. Proin elementum augue at auctor congue. Nulla dapibus hendrerit eros vitae porta. Aliquam sit amet nisi ex. In cursus finibus orci, quis dapibus ex varius vitae. Aliquam erat volutpat. Nullam in maximus nunc. Curabitur at varius eros. ", "JetBrains Mono", 20, (GColor){1.0, 0.0, 0.0, 1.0}, 200);
    GResourceManager_SaveImage(textImage, "text.png");

    printf("Text size is %f %f\n", size.width, size.height);
    printf("textImage size is %lu %lu\n", textImage.width, textImage.height);

    GWindow* window = GWindowManager_OpenWindow();
    app.rootWindow = window;
    printf("WINDOW IS %llu\n", window);


    GView* viewA = GViewManager_InitView();
    window->rootView = viewA;

    return GApplicationManager_Run(&app);
}