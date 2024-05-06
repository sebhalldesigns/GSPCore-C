#ifndef GSHADER_H
#define GSHADER_H

typedef void* GShader;

typedef enum {
    UNSPECIFIED,
    TEXTURE
} GSystemShader;

GShader GShader_AllocSystemShader(GSystemShader systemShader);

void GShader_Free(GShader shader);

#endif // GSHADER_H