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


void GView_Render(GView view) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    GRenderer_RenderSelf(viewDef->renderer);
    GWindow_Render(viewDef->parentWindow);
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
    GView_UpdateParentWindow(subview, viewDef->parentWindow);
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
    viewDef->lastMouseLocation = mouseLocation;
    GView_Render(view);

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

void GView_UpdateParentWindow(GView view, GWindow parentWindow) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    viewDef->parentWindow = parentWindow;
    size_t subviewCount = GVector_Size(viewDef->subviews);
    for (size_t i = 0; i < subviewCount; i++) {
        GView_UpdateParentWindow(GVector_Get(viewDef->subviews, i), parentWindow);
    }
}


void GView_UpdateLayout(GView view) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    size_t subviewCount = GVector_Size(viewDef->subviews);
    GView_Render(viewDef);

    switch (viewDef->layout) {
        case LAYOUT_NONE:
            break;
        case LAYOUT_DOCK:

            float consumedLeft = 0.0;
            float consumedTop = 0.0;
            float consumedRight = 0.0;
            float consumedBottom = 0.0;

            for (size_t i = 0; i < subviewCount; i++) {
                
                GViewDef* subview = GVector_Get(viewDef->subviews, i);

                if (subview != NULL) {


                    if (i == subviewCount - 1) {
                        // i.e this is the last subview, so make it fill the remaining space
                        subview->frame.origin = (GPoint) { viewDef->frame.origin.x + consumedLeft, viewDef->frame.origin.y + consumedTop };
                        subview->frame.size = (GSize) { viewDef->frame.size.width - (consumedLeft + consumedRight), viewDef->frame.size.height - (consumedTop + consumedBottom) }; 
                    } else {
                        // n.b dock left is the default case so there isn't any undefined behaviour
                        switch (subview->dockLocation) {
                            case DOCK_TOP:
                                subview->frame.origin = (GPoint) { viewDef->frame.origin.x + consumedLeft, viewDef->frame.origin.y + consumedTop };
                                subview->frame.size = (GSize) { viewDef->frame.size.width - (consumedLeft + consumedRight), subview->frame.size.height }; 
                                consumedTop += subview->frame.size.height;
                                break;
                            case DOCK_RIGHT:
                                subview->frame.origin = (GPoint) { viewDef->frame.origin.x + (viewDef->frame.size.width - (consumedRight + subview->frame.size.width)), viewDef->frame.origin.y + consumedTop };
                                subview->frame.size = (GSize) { subview->frame.size.width, viewDef->frame.size.height - (consumedTop + consumedBottom) }; 
                                consumedRight += subview->frame.size.width;
                                break;
                            case DOCK_BOTTOM:
                                subview->frame.origin = (GPoint) { viewDef->frame.origin.x + consumedLeft, viewDef->frame.origin.y + ( viewDef->frame.size.height - (consumedBottom + subview->frame.size.height)) };
                                subview->frame.size = (GSize) { viewDef->frame.size.width - (consumedLeft + consumedRight), subview->frame.size.height }; 
                                consumedBottom += subview->frame.size.height;
                                break;
                            default:
                                subview->frame.origin = (GPoint) { viewDef->frame.origin.x + consumedLeft, viewDef->frame.origin.y + consumedTop };
                                subview->frame.size = (GSize) { subview->frame.size.width, viewDef->frame.size.height - (consumedTop + consumedBottom) }; 
                                consumedLeft += subview->frame.size.width;
                                break;
                            

                        }
                    }
                }

                
            }

            break;
        case LAYOUT_STACK:
            break;
    }


    for (size_t i = 0; i < subviewCount; i++) {
        GView_UpdateLayout(GVector_Get(viewDef->subviews, i));
    }
}

void GView_SetDrawing(GView view, GDrawing drawing) {
    if (view == NULL) {
        return;
    }

    GViewDef* viewDef = (GViewDef*)view;
    viewDef->drawing = drawing;
}
