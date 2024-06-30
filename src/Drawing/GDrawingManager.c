#include <GSPCore/GSPCore.h>
#include <GSPCore/Drawing/GDrawingManager.h>


#ifdef GSPCORE_BUILD_UNIX
#include <GSPCore/Drawing/Backends/Cairo/GCairoDrawingManager.h>
#elif GSPCORE_BUILD_WIN32
#include <GSPCore/Drawing/Backends/Direct2D/GDirect2DDrawingManager.h>
#elif GSPCORE_BUILD_MACOS

#elif GSPCORE_BUILD_WEB

#endif

static GDrawingManagerBackend drawingManagerBackend = DRAWING_BACKEND_NONE;

bool GDrawingManager_Init() {
    GApplicationEnvironment environment = GApplicationManager_GetEnvironment();

    switch (environment) {

    #ifdef GSPCORE_BUILD_UNIX
        case ENVIRONMENT_UNIX:
            if (GCairoDrawingManager_Init()) {
                drawingManagerBackend = DRAWING_BACKEND_CAIRO;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WIN32
        case ENVIRONMENT_WIN32:
            if (GDirect2DDrawingManager_Init()) {
                drawingManagerBackend = DRAWING_BACKEND_DIRECT2D;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_MACOS
        case ENVIRONMENT_MACOS:

            return false;
    #elif GSPCORE_BUILD_WEB
        case ENVIRONMENT_WEB:

            return false;
    #endif
        default:
            return false;
    }

}

bool GDrawingManager_SetupDrawing(GDrawing* drawing) {

    switch (drawingManagerBackend) {

        #ifdef GSPCORE_BUILD_UNIX
        case DRAWING_BACKEND_CAIRO:
            return GCairoDrawingManager_SetupDrawing(drawing);
        #elif GSPCORE_BUILD_WIN32
         case DRAWING_BACKEND_DIRECT2D:
            return GDirect2DDrawingManager_SetupDrawing(drawing);
        #endif


        default:
            return false;
    }
}

void GDrawingManager_RenderDrawing(GDrawing* drawing) {

}
