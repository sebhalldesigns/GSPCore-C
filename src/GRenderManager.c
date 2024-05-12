#include "GSPCore/GRenderManager.h"
#include "GSPCore/GHiperRenderManager.h"

bool hasInit = false;
bool isUsingHiper = false;

void GRenderManager_Setup(GRenderManagerMode mode) {

    if (mode != LEGACY_ONLY) {
        isUsingHiper = GHiperRenderManager_TrySetup(mode);
    }

    if (!isUsingHiper) {
        // set up legacy
    }

    hasInit = true;
}

void GRenderManager_SetupWindow(GWindow window) {

    if (isUsingHiper && GHiperRenderManager_TrySetupWindow(window)) {
        return;
    }

    // set up legacy for window
    
}

void GRenderManager_RenderWindow(GWindow window) {
    
    if (isUsingHiper) {
        GHiperRenderManager_RenderWindow(window);
    } else {
        // render legacy
    }
}

void GRenderManager_CleanupWindow(GWindow window) {

    GHiperRenderManager_CleanupWindow(window);
    
        // set up legacy for window
    
}

void GRenderManager_Cleanup() {

    if (hasInit && isUsingHiper) {
        GHiperRenderManager_Cleanup();
    } else if (hasInit) {
        // clean up legacy
    } 

}