#include "GSPCore/Graphics/GTextProvider.h"
#include "internal/def/Graphics/GTextProviderDef.h"

#include "GSPCore/Graphics/GTexture.h"
#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <time.h>

#include <cairo.h>
#include <GL/glew.h>


const static int measureSufaceWidth = 1;
const static int measureSufaceHeight = 1; 

GTextProvider GTextProvider_Alloc(char* typeface, float size) {

    GTextProviderDef* textProvider = calloc(1, sizeof(GTextProviderDef));
    DEBUG_LOG(INFO, "Allocated GTextProviderDef at %lu", (uintptr_t)textProvider);

    textProvider->typeface = typeface;
    textProvider->size = size;
    textProvider->measureSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, measureSufaceWidth, measureSufaceHeight);
    textProvider->measureContext = cairo_create(textProvider->measureSurface);

    cairo_select_font_face(textProvider->measureContext, typeface, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(textProvider->measureContext, size);

    return (GTextProvider)textProvider;
}

void GTextProvider_Free(GTextProvider textProvider) {
    if (textProvider == NULL) {
        return;
    }

    free((GTextProviderDef*)textProvider);
}

GTextMeasurements GTextProvider_MeasureText(GTextProvider textProvider, char* text) {
    if (textProvider == NULL) {
        return (GTextMeasurements) { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    }

    cairo_t* measureContext = ((GTextProviderDef*)textProvider)->measureContext;

    cairo_text_extents_t textExtents;

    cairo_text_extents(measureContext, text, &textExtents);

    return (GTextMeasurements) {
        textExtents.width,
        textExtents.height,
        textExtents.x_bearing,
        textExtents.y_bearing,
        textExtents.x_advance,
        textExtents.y_advance
    };
}

GTexture GTextProvider_Render(GTextProvider textProvider, char* text) {
    if (textProvider == NULL) {
        return NULL;
    }

    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used;
    start = clock();

    GTextProviderDef* provider = (GTextProviderDef*)textProvider;

    cairo_text_extents_t textExtents;

    cairo_text_extents(provider->measureContext, text, &textExtents);

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, textExtents.width, textExtents.height);
    cairo_t* cr = cairo_create(surface);

        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White color
    cairo_paint(cr);

    cairo_select_font_face(cr,"Ubuntu Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, provider->size);
    cairo_set_source_rgb(cr, 0.0, 0.0, 1.0); // Black color

    cairo_move_to(cr, -textExtents.x_bearing, -textExtents.y_bearing);
    cairo_show_text(cr, text);

    cairo_destroy(cr);

    GTexture texture = GTexture_AllocFromData(textExtents.width, textExtents.height, cairo_image_surface_get_data(surface));

    //cairo_surface_write_to_png(surface, "output.png");
    cairo_surface_destroy(surface);

    end = clock();

    cpu_time_used = ((double) (end - start));

    printf("Function took %f seconds to execute.\n", cpu_time_used);



    return texture;
}
