#ifndef GSP_GEOMETRY_TYPES_H
#define GSP_GEOMETRY_TYPES_H

typedef float gfloat_t;

typedef struct {
    gfloat_t width;
    gfloat_t height;
} gsize_t;

typedef struct {
    gfloat_t x;
    gfloat_t y;
} gpoint_t;

typedef struct {
    gpoint_t origin;
    gsize_t size;
} grect_t;

grect_t grect_init(gfloat_t x, gfloat_t y, gfloat_t width, gfloat_t height);

#endif // GSP_GEOMETRY_TYPES_H