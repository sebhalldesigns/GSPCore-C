#include "GSPCore/Graphics/GFrame.h"
#include "internal/def/Graphics/GFrameDef.h"
#include "internal/include/GVector.h"

#include "GSPCore/GLog.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>



// Vertex shader source code
const static char* vertexShaderSource = "                      \
    #version 330 core                                   \
    layout (location = 0) in vec3 aPos;                 \
    layout (location = 1) in vec2 aTexCoord;            \
                                                        \
    out vec2 TexCoord;                                  \
                                                        \
    void main() {                                       \
        gl_Position = vec4(aPos, 1.0);                  \
        TexCoord = aTexCoord;                           \
    }                                                   \
";

// Fragment shader source code
const static char* fragmentShaderSource = "                    \
    #version 330 core                                   \
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

// Define the vertices and texture coordinates of the quad
const static float vertices[] = {
    // Positions          // Texture Coordinates
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left 
};
const static unsigned int indices[] = {
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};

static uint32_t VBO = 0;
static uint32_t VAO = 0;
static uint32_t EBO = 0;

static uint32_t SimpleShader = 0;

GFrame GFrame_Alloc(GFrameInfo info) {

    if (SimpleShader == 0) {

        if (glewInit() != GLEW_OK) {
            DEBUG_LOG(FAIL, "Failed to initialize GLEW");
            return NULL;
        }

        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);


        // Bind VBO and copy vertices into it
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind EBO and copy indices into it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        // Unbind VAO
        glBindVertexArray(0);

        // Create and compile vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Create and compile fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Create shader program
        SimpleShader = glCreateProgram();
        glAttachShader(SimpleShader, vertexShader);
        glAttachShader(SimpleShader, fragmentShader);
        glLinkProgram(SimpleShader);

        // Delete shaders (no longer needed)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        DEBUG_LOG(INFO, "Set up OpenGL successfully");
    
    }

    GFrameDef* frame = calloc(1, sizeof(GFrameDef));
    glGenTextures(1, &frame->glBuffer);
    glBindTexture(GL_TEXTURE_2D, frame->glBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    DEBUG_LOG(INFO, "Allocated GFrame at %lu", (uintptr_t)frame);
    
    return (GFrame)frame;
}

void GFrame_Free(GFrame frame) {
    glDeleteTextures(1, &((GFrameDef*)frame)->glBuffer);
    free((GFrameDef*)frame);

    DEBUG_LOG(INFO, "Freed GFrame at %lu", (uintptr_t)frame);
}

void GFrame_Fill(GFrame frame, GRect rect, GColor color) {
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ((GFrameDef*)frame)->glBuffer, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }
    

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0, ((GFrameDef*)frame)->width, ((GFrameDef*)frame)->height); 
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}