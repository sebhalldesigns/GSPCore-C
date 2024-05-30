#include "GSPCore/GSPCore.h"
#include "internal/def/GApplicationDef.h"

#include "GSPCore/GLog.h"
#include "internal/def/GWindowDef.h"

#include "GSPCore/GRenderManager.h"
#include "GSPCore/GHiperRenderManager.h"

#include "GSPCore/GWebGLRenderer.h"


#include <stdlib.h>
#include <stdio.h>

#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void MouseMove(double mouseX, double mouseY) {
    //printf("%f %f\n", mouseX, mouseY);
    //GWebGLRenderer_RenderView(NULL);

}

EMSCRIPTEN_KEEPALIVE
void WindowResize(double width, double height) {
    //printf("%f %f\n", mouseX, mouseY);
    GWebGLRenderer_SetViewportSize(width, height);
    GWebGLRenderer_RenderView(NULL);

}



void main_loop() {
    // do nothing here
}

int GSPRun(GApplication app) {

    GLog(INFO, "Starting GSPCore on WASM...");

    
    //GRenderManager_Setup(DEFAULT);

    GWebGLRenderer_Init();

    EM_ASM({
        addEventListener("mousemove", (event) => {
            Module.ccall("MouseMove", null, ["float", "float"], [event.clientX, event.clientY])
        });

        window.addEventListener("resize", (event) => {
            let canvasElement = document.getElementById("canvas");
            canvasElement.width = window.innerWidth;
            canvasElement.height = window.innerHeight;
        });

        window.addEventListener("resize", (event) => {
            Module.ccall("WindowResize", null, ["float", "float"], [window.innerWidth, window.innerHeight]);
        });

            let canvasElement = document.getElementById("canvas");
            canvasElement.width = window.innerWidth;
            canvasElement.height = window.innerHeight;
         Module.ccall("WindowResize", null, ["float", "float"], [window.innerWidth, window.innerHeight]);
    });


    if (app == 0 || ((GApplicationDef*)app)->launchEvent == 0) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }

    //if (!GWindowDef_Init()) {
    //    GLog(FAIL, "GSPCore initialisation failed! Exiting application.");
    //    return -1;
    //}


    (((GApplicationDef*)app)->launchEvent)();

     //GRenderManager_Cleanup();
    emscripten_set_main_loop(main_loop, 0, 1);


    GLog(INFO, "No windows open. Exiting application.");

    return 0;
}