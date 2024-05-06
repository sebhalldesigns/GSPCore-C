#include <stdio.h>
#include <cairo.h>
#include <time.h>


int main() {
    // Dimensions of the image
    int width = 400;
    int height = 200;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    // Create a Cairo surface for writing to an image file
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Draw background (optional)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White color
    cairo_paint(cr);

    // Set font options
    cairo_select_font_face(cr, "Ubuntu Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black color

    // Draw text
    cairo_move_to(cr, 50, 100);
    cairo_show_text(cr, "Hello, Cairo!");

    // Clean up
    cairo_destroy(cr);

    end = clock();

    // Save the surface to a PNG file
    cairo_surface_write_to_png(surface, "output.png");

    // Destroy the surface
    cairo_surface_destroy(surface);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Function took %f seconds to execute.\n", cpu_time_used);


    return 0;
}