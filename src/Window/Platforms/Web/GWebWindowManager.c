#include <GSPCore/GSPCore.h>
#include <GSPCore/Window/Platforms/Web/GWebWindowManager.h>
#include <GSPCore/Render/GRenderManager.h>

#include <stdbool.h>

bool GWebWindowManager_TryInit() {

    EM_ASM({
        addEventListener("mousemove", (event) => {
            Module.ccall("GWebWindowManager_MouseMoveCallback", null, ["float", "float"], [event.clientX, event.clientY])
        });


        window.addEventListener("resize", function (event) {
            const newWidth = window.innerWidth;
            const newHeight = window.innerHeight;

            const canvasElement = document.getElementById("canvas");

            canvasElement.width = newWidth;
            canvasElement.height = newHeight;
            canvasElement.style.width = newWidth;
            canvasElement.style.height = newHeight;

            Module.ccall("GWebWindowManager_ResizeCallback", null, ["float", "float"], [newWidth, newHeight]);

            
        });

        window.matchMedia('(prefers-color-scheme: dark)')
            .addEventListener('change',({ matches }) => {
            if (matches) {
                Module.ccall("GWebWindowManager_DarkModeCallback", null, null, null);
            } else {
                Module.ccall("GWebWindowManager_LightModeCallback", null, null, null);
            }
        });


        const newWidth = window.innerWidth;
        const newHeight = window.innerHeight;

        const canvasElement = document.getElementById("canvas");
        canvasElement.width = newWidth;
        canvasElement.height = newHeight;
        canvasElement.style.width = newWidth;
        canvasElement.style.height = newHeight;

        Module.ccall("GWebWindowManager_ResizeCallback", null, ["float", "float"], [newWidth, newHeight]);

    });

    return true;
}
GWindow* GWebWindowManager_OpenWindow() {
    return NULL;
}
int GWebWindowManager_RunLoop(GApplication* app) {
    return -1;
}


void GWebWindowManager_MouseMoveCallback(double mouseX, double mouseY) {
    printf("MOUSE MOVE\n");
}

void GWebWindowManager_ResizeCallback(double width, double height) {
    printf("RESIZE\n");
}

void GWebWindowManager_LightModeCallback() {
    printf("LIGHT MODE\n");
}

void GWebWindowManager_DarkModeCallback() {
    printf("DARK MODE\n");
}