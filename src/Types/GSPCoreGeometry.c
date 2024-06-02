#include "GSPCore/Types/GSPCoreGeometry.h"

bool GRect_ContainsPoint(GRect rect, GPoint point) {
    return (rect.origin.x < point.x 
        && rect.origin.y < point.y 
        && point.x < (rect.origin.x + rect.size.width) 
        && point.y < (rect.origin.y + rect.size.height)); 
}