#include "GSPCore/Graphics/GVertexBuffer.h"
#include "internal/def/Graphics/GVertexBufferDef.h"

#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <GL/glew.h>

GVertexBuffer GVertexBuffer_Alloc(float* vertexData, size_t vertexDataLength, uint32_t* indexData, size_t indexDataLength) {


    GVertexBufferDef* buffer = calloc(1, sizeof(GVertexBufferDef));

    DEBUG_LOG(INFO, "Allocated GVertexBuffer at %lu", (uintptr_t)buffer);

    glGenVertexArrays(1, &buffer->glVertexArrayBuffer);

    glGenBuffers(1, &buffer->glVertexBuffer);
    glGenBuffers(1, &buffer->glElementBuffer);
    glBindVertexArray(buffer->glVertexArrayBuffer);

    // Bind VBO and copy vertices into it
    glBindBuffer(GL_ARRAY_BUFFER, buffer->glVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexDataLength*sizeof(float), vertexData, GL_STATIC_DRAW);

    // Bind EBO and copy indices into it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->glElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataLength*sizeof(uint32_t), indexData, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);
    
    return buffer;
}

void GVertexBuffer_Free(GVertexBuffer buffer) {
    if (buffer == NULL) {
        return;
    }

    glDeleteVertexArrays(1, &((GVertexBufferDef*)buffer)->glVertexArrayBuffer);
    glDeleteBuffers(1, &((GVertexBufferDef*)buffer)->glVertexBuffer);
    glDeleteBuffers(1, &((GVertexBufferDef*)buffer)->glElementBuffer);
}
