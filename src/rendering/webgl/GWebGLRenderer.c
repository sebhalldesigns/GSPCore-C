#include "GSPCore/GWebGLRenderer.h"

#include "internal/def/UI/GViewDef.h"


#include "GSPCore/GLog.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <time.h>

static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = 0;

bool GWebGLRenderer_Init() {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2; // Request a WebGL2 context

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
    if (ctx <= 0) {
        GLog(FAIL, "Failed to make WebGL2 context!");
        return false;
    }

    context = ctx;
    emscripten_webgl_make_context_current(context);

    GLog(INFO, "Created WebGL2 Context");

    return true;
}

void GWebGLRenderer_RenderView(GView view) {

    GLog(INFO, "Starting WebGL Render...");
    clock_t then = clock();

    // Clear the screen with a color    
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    GViewDef* viewDef = (GViewDef*) view;
    
    

    
    clock_t now = clock();

    double frameTime = ((double) (now - then));

    GLog(INFO, "Completed WebGL Render in %f us", frameTime);
    
}