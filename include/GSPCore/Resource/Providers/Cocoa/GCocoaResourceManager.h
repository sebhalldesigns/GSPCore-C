#ifndef GCOCOARESOURCEMANAGER_H
#define GCOCOARESOURCEMANAGER_H

#include <stdbool.h>
#include <GSPCore/Types/GSPCoreOpaqueTypes.h>
#include <GSPCore/Types/GSPCoreGraphics.h>
#include <GSPCore/Types/GSPCoreGeometry.h>

bool GCocoaResourceManager_Init();
GRawImage GCocoaResourceManager_LoadImage(const char* path);
void GCocoaResourceManager_SaveImage(GRawImage image, char* path);
GSize GCocoaResourceManager_MeasureText(const char* text, const char* typeface, float size);
GRawImage GCocoaResourceManager_LoadText(const char* text, const char* typeface, float size, GColor color, float maxWidth);

#endif // GCOCOARESOURCEMANAGER_H