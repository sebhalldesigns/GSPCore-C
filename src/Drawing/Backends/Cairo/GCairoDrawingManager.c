#include <GSPCore/GSPCore.h>
#include <GSPCore/Drawing/Backends/Cairo/GCairoDrawingManager.h>

#include <cairo.h>

bool GCairoDrawingManager_Init() {
    
    cairo_surface_t* testSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100, 100);

    if (cairo_surface_status(testSurface) != CAIRO_STATUS_SUCCESS) {
        GLog(FAIL, "Failed to create a valid cairo surface!");
        return false;
    }

    cairo_surface_destroy(testSurface);
    
    GLog(INFO, "Started GDrawingManager on Cairo backend");

    return true;
}

bool GCairoDrawingManager_SetupDrawing(GDrawing* drawing) {
    


    return true;
}

void GCairoDrawingManager_RenderDrawing(GDrawing* drawing) {

}