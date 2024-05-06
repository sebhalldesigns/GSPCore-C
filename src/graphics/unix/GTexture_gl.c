#include "GSPCore/Graphics/GTexture.h"
#include "internal/def/Graphics/GTextureDef.h"

#include "GSPCore/GLog.h"

#include <math.h>
#include <GL/glew.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GTexture GTexture_AllocFromFile(const char* filePath) {
    GTextureDef* texture = calloc(1, sizeof(GTextureDef));

    DEBUG_LOG(INFO, "Allocated GTexture at %lu", (uintptr_t)texture);

    // Load and create a texture
    glGenTextures(1, &texture->glBuffer);
    glBindTexture(GL_TEXTURE_2D, texture->glBuffer);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data and generate texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        DEBUG_LOG(INFO, "Loaded image from %s", filePath);
    } else {
        DEBUG_LOG(ERROR, "Failed to load image from %s", filePath);
    }
    stbi_image_free(data);

    return texture;

}

GTexture GTexture_AllocFromData(uint32_t width, uint32_t height, uint8_t* data) {
    GTextureDef* texture = calloc(1, sizeof(GTextureDef));

    DEBUG_LOG(INFO, "Allocated GTexture at %lu", (uintptr_t)texture);

    // Load and create a texture
    glGenTextures(1, &texture->glBuffer);
    glBindTexture(GL_TEXTURE_2D, texture->glBuffer);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
        DEBUG_LOG(INFO, "Loaded texture from data");
    } else {
        DEBUG_LOG(ERROR, "Failed to load texture from data");
    }

    return texture;

}

void GTexture_Free(GTexture texture) {

}
