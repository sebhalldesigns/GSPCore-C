#include "GSPCore/GSPCore.h"

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


        window.addEventListener("resize", function (event) {
            const newWidth = window.innerWidth;
            const newHeight = window.innerHeight;

            const canvasElement = document.getElementById("canvas");

            canvasElement.width = newWidth;
            canvasElement.height = newHeight;
            canvasElement.style.width = newWidth;
            canvasElement.style.height = newHeight;

            Module.ccall("WasmWindowResize", null, ["float", "float"], [newWidth, newHeight]);

            
        });

        window.matchMedia('(prefers-color-scheme: dark)')
            .addEventListener('change',({ matches }) => {
            if (matches) {
                Module.ccall("WasmDarkMode", null, null, null);
            } else {
                Module.ccall("WasmLightMode", null, null, null);
            }
        });


        const newWidth = window.innerWidth;
        const newHeight = window.innerHeight;

        const canvasElement = document.getElementById("canvas");
        canvasElement.width = newWidth;
        canvasElement.height = newHeight;
        canvasElement.style.width = newWidth;
        canvasElement.style.height = newHeight;

        Module.ccall("WasmWindowResize", null, ["float", "float"], [newWidth, newHeight]);

    });


    emscripten_set_main_loop(WasmMainLoop, 0, 1);


    GLog(INFO, "No windows open. Exiting application.");

    return 0;
}


void WasmMouseMove(double mouseX, double mouseY) {

    GApplicationDef* appDef = (GApplicationDef*)app;

    if (appDef->mainWindow == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*) appDef->mainWindow;

    if (windowDef->rootView == NULL) {
        return;
    }

    GPoint mouseLocation = (GPoint) { mouseX, mouseY };

    GView_UpdateMouseLocation(windowDef->rootView, mouseLocation);

}


void WasmWindowResize(double width, double height) {

    if (app == NULL) {
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)app;

    if (appDef->mainWindow == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*) appDef->mainWindow;

    if (windowDef->controller == NULL) {
        return;
    }

    GWindowControllerDef* controllerDef = (GWindowControllerDef*)windowDef->controller;

    if (controllerDef->resizeEvent != NULL) {
        (controllerDef->resizeEvent)(windowDef, (GSize) { (float) width, (float) height });
    }

    windowDef->info.size = (GSize) { (float) width, (float) height };

    GWindow_Layout(windowDef);

    GWindow_Render(windowDef);
    /*
    EM_ASM({
        const canvasElement = document.getElementById("canvas");
        canvasElement.width = 100;
        canvasElement.height = 200;
        console.log('I received: ' + $0 + ' ' + $1);
    }, width, height);
*/
    //    emscripten_set_canvas_size( width, height);

    //emscripten_set_canvas_element_size("#canvas", width, height);
}



void WasmMainLoop() {
    // do nothing here
}


void WasmLightMode() {
    printf("LIGHT MODE\n");
}

void WasmDarkMode() {
    printf("DARK MODE\n");
}