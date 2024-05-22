#include "GSPCore/UI/GView.h"
#include "internal/def/UI/GViewDef.h"

#include "GSPCore/GLog.h"

#include <stdlib.h>

GView GView_Alloc(GViewInfo info) {

    GView view = calloc(1, sizeof(GViewDef));

    if (view == NULL) {
        DEBUG_LOG(FAIL, "Failed to allocate GView!");
        return NULL;
    }

    DEBUG_LOG(INFO, "Allocated GView at %lu", view);

    ((GViewDef*)view)->color = info.color;
    ((GViewDef*)view)->frame = info.frame;

    return view;

}