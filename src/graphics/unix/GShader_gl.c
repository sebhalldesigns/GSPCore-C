#include "GSPCore/Graphics/GShader.h"
#include "internal/def/Graphics/GShaderDef.h"

#include "GSPCore/GLog.h"

#include <GL/glew.h>
#include <stdlib.h>


static char errorMessageBuffer[1024];
void CheckShaderCompilationErrors(uint32_t shader);
void CheckProgramLinkErrors(uint32_t program);

const static char* SystemTextureVertexShader = "        \
    #version 330 core\n                                 \
    layout (location = 0) in vec3 aPos;                 \
    layout (location = 1) in vec2 aTexCoord;            \
                                                        \
    out vec2 TexCoord;                                  \
                                                        \
    void main() {                                       \
        gl_Position = vec4(aPos, 1.0);                  \
        TexCoord = vec2(aTexCoord.x, aTexCoord.y);      \
    }                                                   \
";

const static char* SystemTextureFragmentShader = "      \
    #version 330 core\n                                 \
    out vec4 FragColor;                                 \
                                                        \
    in vec2 TexCoord;                                   \
                                                        \
    uniform sampler2D textureSampler;                   \
                                                        \
    void main() {                                       \
        FragColor = texture(textureSampler, TexCoord);  \
    }                                                   \
";

GShader GShader_AllocSystemShader(GSystemShader systemShader) {
    
    GShaderDef* shader = calloc(1, sizeof(GShaderDef));

    DEBUG_LOG(INFO, "Allocated GShader at %lu", (uintptr_t)shader);


    uint32_t vertexShader, fragmentShader;

    switch (systemShader) {
        case UNSPECIFIED:
            DEBUG_LOG(FAIL, "Unspecified shader requested!");
            return NULL;
        case TEXTURE:
            // vertex
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &SystemTextureVertexShader, NULL);
            glCompileShader(vertexShader);
            CheckShaderCompilationErrors(vertexShader);
            // fragment
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &SystemTextureFragmentShader, NULL);
            glCompileShader(fragmentShader);
            CheckShaderCompilationErrors(fragmentShader);
            // program
            shader->glShaderProgram = glCreateProgram();
            glAttachShader(shader->glShaderProgram, vertexShader);
            glAttachShader(shader->glShaderProgram, fragmentShader);
            glLinkProgram(shader->glShaderProgram);
            CheckProgramLinkErrors(shader->glShaderProgram);
            // cleanup
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            break;
    }

    
    return shader;

}

void GShader_Free(GShader shader) {

}

void CheckShaderCompilationErrors(uint32_t shader) {
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, 1024, NULL, errorMessageBuffer);
        DEBUG_LOG(FAIL, "OpenGL Shader compilation failed: %s", errorMessageBuffer);
    }
}
void CheckProgramLinkErrors(uint32_t program) {
    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(program, 1024, NULL, errorMessageBuffer);
        DEBUG_LOG(FAIL, "OpenGL Program linking failed: %s", errorMessageBuffer);
    }
}