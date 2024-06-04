#ifndef GPATH_H
#define GPATH_H

#include "../Types/GSPCoreOpaqueTypes.h"
#include "../Types/GSPCoreGeometry.h"

typedef enum {
    PATH_SEGMENT_LINE,
    PATH_SEGMENT_CONIC,
    PATH_SEGMENT_QUADRATIC_BEZIER,
    PATH_SEGMENT_CUBIC_BEZIER
} GPathSegmentType;

typedef struct {
    GPathSegmentType type;
    GPoint start;// possibly redundant, have a chain rahter than independant segments.
    GPoint end;
    GPoint ctrlA;
    GPoint ctrlB;
} GPathSegment;

GPath GPath_Init(GPoint start);

void GPath_AddLineSegment(GPoint toPoint);
void GPath_AddConicSegment(GPoint endPoint, GPoint center, GFloat angle);
void GPath_AddQuadraticBezierSegment(GPoint endPoint, GPoint control);
void GPath_AddCubicBezierSegment(GPoint endPoint, GPoint controlA, GPoint controlB);




#endif // GPATH_H