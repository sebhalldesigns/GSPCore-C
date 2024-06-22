#include <GSPCore/View/GViewManager.h>
#include <GSPCore/View/GView.h>
#include <GSPCore/GSPCore.h>

bool GViewManager_Init() {

    GLog(INFO, "Started GViewManager");
    return true;
}

GView* GViewManager_InitView() {
    GView* view = calloc(1, sizeof(GView));
    view->nextSibling = NULL;
    view->numSubviews = 0;
    view->rootSubview = NULL;

    return view;
}