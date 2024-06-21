#ifndef GRESOURCEMANAGER_H
#define GRESOURCEMANAGER_H

#include <stdbool.h>
#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <GSPCore/Types/GSPCoreGraphics.h>
#include <GSPCore/Types/GSPCoreGeometry.h>

typedef enum {
    PROVIDER_NONE,
    PROVIDER_CAIRO,
    PROVIDER_COCOA,
    PROVIDER_GDI,
    PROVIDER_IOS,
    PROVIDER_ANDROID,
    PROVIDER_SDL
} GResourceManagerProvider;

bool GResourceManager_Init();
GRawImage GResourceManager_LoadImage(const char* path);
void GResourceManager_SaveImage(GRawImage image, char* path);
GSize GResourceManager_MeasureText(const char* text);
GRawImage GResourceManager_LoadText(const char* text);

#endif // GRESOURCEMANAGER_H