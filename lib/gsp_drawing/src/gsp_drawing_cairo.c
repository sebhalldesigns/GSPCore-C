#include <cairo/cairo.h>

// Create a Cairo surface and context
int width = 512;
int height = 512;
cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
cairo_t* cr = cairo_create(surface);

// Draw something with Cairo
cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
cairo_paint(cr);

// Finish the Cairo drawing
cairo_destroy(cr);