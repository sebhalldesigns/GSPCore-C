#include <gsp_types/gsp_geometry_types.h>

grect_t grect_init(gfloat_t x, gfloat_t y, gfloat_t width, gfloat_t height) {
    return (grect_t) { (gpoint_t) { x, y }, (gsize_t) { width, height } };
}
