#include "GSPCore/Drawing/GDrawing.h"
#include "GSPCore/GSPCore.h"

GDrawing GDrawing_Init() {

    GDrawingDef* drawingDef = calloc(1, sizeof(GDrawingDef));

    if (drawingDef == NULL) {
        GLog(FAIL, "Failed to allocate GDrawing!");
        return NULL;
    }



    return drawingDef;
}

void GDrawing_Free(GDrawing drawing) {
    free(drawing);
}
