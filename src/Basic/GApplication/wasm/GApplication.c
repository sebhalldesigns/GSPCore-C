#include "GSPCore/Basic/GApplication.h"

#include "GSPCore/Basic/GLog.h"


#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void MainLoop() {

}


GApplication GApplication_Init(GApplicationInfo info) {

    // check if there is already an app
    if (app != NULL) {
        return NULL;
    }

    GApplicationDef* appDef = calloc(1, sizeof(GApplicationDef));

    if (appDef == NULL) {
        return NULL;
    }


    appDef->info = info;

    app = appDef;
    return app;
}

void GApplication_Free(GApplication application) {
    free(application);
}

void GApplication_SetController(GApplication application, GApplicationController controller) {
    if (application == NULL) {
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;

    appDef->controller = controller;

}


GWindow GApplication_GetMainWindow(GApplication application) {

    if (application == NULL) {
        return NULL;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;
    return appDef->mainWindow;
}

void GApplication_SetMainWindow(GApplication application, GWindow window) {

    if (application == NULL) {
        return;
    }
    
    GApplicationDef* appDef = (GApplicationDef*)application;
    appDef->mainWindow = window;

    printf("SET APP DEF AT %lu\n", application);

}


int GApplication_Run(GApplication application) {

    if (application == NULL) {
        printf("APPLICATION NULL\n");
        return -1;
    }
    

    
    GApplicationDef* appDef = (GApplicationDef*)application;

    GApplicationControllerDef* controller = (GApplicationControllerDef*)appDef->controller;

    if (controller->launchEvent == NULL) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    (controller->launchEvent)(application);

    EM_ASM({
        addEventListener("mousemove", (event) => {
            Module.ccall("WasmMouseMove", null, ["float", "float"], [event.clientX, event.clientY])
        });

        window.addEventListener("resize", (event) => {
            let canvasElement = document.getElementById("canvas");
            canvasElement.width = window.innerWidth;
            canvasElement.height = window.innerHeight;
        });

        window.addEventListener("resize", (event) => {
            Module.ccall("WasmWindowResize", null, ["float", "float"], [window.innerWidth, window.innerHeight]);
        });

            let canvasElement = document.getElementById("canvas");
            canvasElement.width = window.innerWidth;
            canvasElement.height = window.innerHeight;
         Module.ccall("WasmWindowResize", null, ["float", "float"], [window.innerWidth, window.innerHeight]);
    });


    emscripten_set_main_loop(WasmMainLoop, 0, 1);


    GLog(INFO, "No windows open. Exiting application.");

    return 0;
}


void WasmMouseMove(double mouseX, double mouseY) {
    printf("%f %f\n", mouseX, mouseY);
    //GWebGLRenderer_RenderView(NULL);


}


void WasmWindowResize(double width, double height) {
    printf("%f %f\n", width, height);
    //GWebGLRenderer_SetViewportSize(width, height);
    //GWebGLRenderer_RenderView(NULL);
    
    if (app == NULL) {
        printf("APP NULL\n");
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)app;

    if (appDef->mainWindow == NULL) {
        printf("MAIN WINDOW NULL AT %lu\n", appDef);
        return;
    }

    GWindowDef* windowDef = (GWindowDef*) appDef->mainWindow;

    if (windowDef->controller == NULL) {
        printf("CONTROLLER NULL\n");
        return;
    }

    GWindowControllerDef* controllerDef = (GWindowControllerDef*)windowDef->controller;

    if (controllerDef->resizeEvent != NULL) {
        (controllerDef->resizeEvent)(windowDef, (GSize) { (float) width, (float) height });
    }

    printf("EVENT NULL\n");
}



void WasmMainLoop() {
    // do nothing here
}