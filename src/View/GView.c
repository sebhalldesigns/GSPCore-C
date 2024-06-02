#include "GSPCore/View/GView.h"
#include "GSPCore/GSPCore.h"

static size_t numViewsAllocated;

GView GView_Init(GViewInfo info) {
    GViewDef* viewDef = calloc(1, sizeof(GViewDef));

    if (viewDef == NULL) {
        GLog(FAIL, "Failed to allocate GView!");
        return NULL;
    }


    numViewsAllocated++;

    GVector vector = GVector_Init();
    
    if (vector == NULL) {
        GLog(FAIL, "Failed to allocate GView->GVector!");
        free(viewDef);
        return NULL;
    }


    GRendererDef* renderer = (GRendererDef*)GRenderer_Init();
    
    if (renderer == NULL) {
        GLog(FAIL, "Failed to allocate GView->GRenderer!");
        free(viewDef);
        return NULL;
    }

    
    

    viewDef->subviews = vector;
    viewDef->renderer = renderer;
    viewDef->allocationIndex = numViewsAllocated;
    viewDef->name = info.name;
    viewDef->frame = info.frame;
    viewDef->backgroundColor = info.backgroundColor;

    if (viewDef->name == NULL || strcmp(viewDef->name, "") == 0) {
        GLog(INFO, "Allocated GView #%lu", viewDef->allocationIndex);
    } else {
        GLog(INFO, "Allocated GView '%s'", viewDef->name);
    }
    
    renderer->view = viewDef;

    return viewDef;
}

void GView_Free(GView view) {
    free(view);
}

void GView_SetController(GView view, GViewController viewController) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    viewDef->controller = viewController;

}

void GView_LayoutChildren() {

}

void GView_Render(GView view) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    GRenderer_RenderSelf(viewDef->renderer);
}

void GView_AddSubview(GView view, GView subview) {
    if (view == NULL || subview == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    GViewDef* subviewDef = (GViewDef*)subview;
    
    if (subviewDef->parentView != NULL) {
        GLog(WARNING, "View already has a parent so can't be added as a subview!");
        return;
    }

    if (GVector_Contains(viewDef->subviews, subview)) {
        GLog(WARNING, "View is already a subview of this view");
        return;
    }

    GVector_Add(viewDef->subviews, subview);
    subviewDef->parentView = view;
}

size_t GView_SubviewCount(GView view) {
    if (view == NULL) {
        return 0;
    }

    GViewDef* viewDef = (GViewDef*)view;
    if (viewDef->subviews == NULL) {
        return 0;
    }

    return GVector_Size(viewDef->subviews);
}

void GView_UpdateMouseLocation(GView view, GPoint mouseLocation) {

    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;

    if (viewDef->controller != NULL && GRect_ContainsPoint(viewDef->frame, mouseLocation)) {

        if (!viewDef->isMouseInside) {
            // trigger mouse enter event    
            if (((GViewControllerDef*)viewDef->controller)->mouseEnterEvent != NULL) {
                (((GViewControllerDef*)viewDef->controller)->mouseEnterEvent)(viewDef);
                viewDef->isMouseInside = true;
            }
        }

    } else if (viewDef->controller != NULL) {
        
        if (viewDef->isMouseInside) {
            // trigger mouse enter event
            if (viewDef->controller != NULL && ((GViewControllerDef*)viewDef->controller)->mouseExitEvent != NULL) {
                (((GViewControllerDef*)viewDef->controller)->mouseExitEvent)(viewDef);
                viewDef->isMouseInside = false;
            }
        }
    }

    size_t subviewCount = GVector_Size(viewDef->subviews);
    for (size_t i = 0; i < subviewCount; i++) {
        GView_UpdateMouseLocation(GVector_Get(viewDef->subviews, i), mouseLocation);
    }


}