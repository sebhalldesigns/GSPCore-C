#ifndef GTEXTURE_H
#define GTEXTURE_H

// GTexture represents a bitmap image, usually loaded from the file system.

#include <stdint.h>

typedef void* GTexture;

GTexture GTexture_AllocFromFile(const char* filePath);
GTexture GTexture_AllocFromData(uint32_t width, uint32_t height, uint8_t* data);
void GTexture_Free(GTexture texture);



#endif // GTEXTURE_H