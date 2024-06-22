#include <GSPCore/GSPCore.h>
#include <GSPCore/Resource/GResourceManager.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifdef GSPCORE_BUILD_UNIX
#elif GSPCORE_BUILD_WIN32
#elif GSPCORE_BUILD_MACOS
#include "GSPCore/Resource/Providers/Cocoa/GCocoaResourceManager.h"
#endif

static GResourceManagerProvider provider = PROVIDER_NONE;


bool GResourceManager_Init() {
    GApplicationEnvironment environment = GApplicationManager_GetEnvironment();

    switch (environment) {
    #ifdef GSPCORE_BUILD_UNIX
    #elif GSPCORE_BUILD_MACOS
        case ENVIRONMENT_MACOS:
            if (GCocoaResourceManager_Init()) {
                provider = PROVIDER_COCOA;
                return true;
            }

            return false;
    #elif GSPCORE_BUILD_WEB
    #endif
        default:
            return false;
    }
}

GRawImage GResourceManager_LoadImage(const char* path) {
    switch (provider) {
    #ifdef GSPCORE_BUILD_UNIX
    #elif GSPCORE_BUILD_MACOS
        case PROVIDER_COCOA:
                return GCocoaResourceManager_LoadImage(path);
    #endif
        default:
            return (GRawImage){NULL, 0, 0};
    }
}

void GResourceManager_SaveImage(GRawImage image, char* path) {
    switch (provider) {
    #ifdef GSPCORE_BUILD_UNIX
    #elif GSPCORE_BUILD_MACOS
        case PROVIDER_COCOA:
                GCocoaResourceManager_SaveImage(image, path);
                return;
    #endif
        default:
            return;
    }
}

GSize GResourceManager_MeasureText(const char* text, const char* typeface, float size) {
    switch (provider) {
    #ifdef GSPCORE_BUILD_UNIX
    #elif GSPCORE_BUILD_MACOS
        case PROVIDER_COCOA:
                return GCocoaResourceManager_MeasureText(text, typeface, size);
    #endif
        default:
            return (GSize){0.0, 0.0};
    }
}

GRawImage GResourceManager_LoadText(const char* text, const char* typeface, float size, GColor color, float maxWidth) {
    switch (provider) {
    #ifdef GSPCORE_BUILD_UNIX
    #elif GSPCORE_BUILD_MACOS
        case PROVIDER_COCOA:
                return GCocoaResourceManager_LoadText(text, typeface, size, color, maxWidth);
    #endif
        default:
            return (GRawImage){NULL, 0, 0};
    }
}