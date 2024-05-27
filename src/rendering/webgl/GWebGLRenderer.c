#include "GSPCore/GWebGLRenderer.h"
#include "GSPCore/UI/GView.h"

#include "internal/def/UI/GViewDef.h"


#include "GSPCore/GLog.h"

#include <stdio.h>
#include <stdbool.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <time.h>


#define NUM_VIEWS 100

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

double viewportWidth = 800;
double viewportHeight = 600;

float ortho[16];

GLuint program;

const char *vertexShaderSource = "\
#version 300 es\n\
layout (location = 0) in vec2 aPos;\n\
\n\
out vec2 TexCoord;\n\
\n\
uniform vec2 uPos;\n\
uniform vec2 uSize;\n\
uniform vec2 uViewportSize;\n\
uniform mat4 uProjectionMatrix; // Orthographic projection matrix uniform \n\
\n\
void main() {\n\
    vec2 normalised = vec2((aPos*uSize)/uViewportSize);\n\
    gl_Position = vec4(normalised * 2.0 - 1.0, 0.0, 1.0);\n\
    TexCoord = aPos;\n\
}\n\
";

const char *fragmentShaderSource = "\
#version 300 es\n\
precision mediump float;\n\
\n\
in vec2 TexCoord;\n\
out vec4 FragColor;\n\
\n\
uniform sampler2D uTexture;\n\
\n\
void main() {\n\
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);//texture(uTexture, TexCoord);\n\
}\n\
";

float vertices[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

GLuint VAO, VBO;
GLint uPos, uSize, uViewportSize, uProjectionMatrix, uTexture;


GLuint compileShader(GLenum type, const char *source) {

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        GLog(FAIL, "Failed to compile WebGL shader\n%s\n", infoLog);
    }
    return shader;
}


bool GWebGLRenderer_Init() {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2; // Request a WebGL2 context

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
    if (ctx <= 0) {
        GLog(FAIL, "Failed to make WebGL2 context!");
        return false;
    }

    context = ctx;
    emscripten_webgl_make_context_current(context);

    GLog(INFO, "Created WebGL2 Context");

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        GLog(FAIL, "Failed to create WebGL shader program!");
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    program = shaderProgram;
    glUseProgram(program);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    uPos = glGetUniformLocation(shaderProgram, "uPos");
    uSize = glGetUniformLocation(shaderProgram, "uSize");
    uViewportSize = glGetUniformLocation(shaderProgram, "uViewportSize");
    uProjectionMatrix = glGetUniformLocation(shaderProgram, "uProjectionMatrix");
    uTexture = glGetUniformLocation(shaderProgram, "uTexture");
    
    return true;
}


void GWebGLRenderer_RenderView(GView view) {

    GLog(INFO, "Starting WebGL Render...");
    clock_t then = clock();

    float right = 800.0;
    float left = 0.0;
    float top = 0.0;
    float bottom = 600.0;
    float far = 1.0;
    float near = -1.0;
    ortho[0] = 2.0f / (right - left);
    ortho[1] = 0.0f;
    ortho[2] = 0.0f;
    ortho[3] = 0.0f;

    ortho[4] = 0.0f;
    ortho[5] = 2.0f / (top - bottom);
    ortho[6] = 0.0f;
    ortho[7] = 0.0f;

    ortho[8] = 0.0f;
    ortho[9] = 0.0f;
    ortho[10] = -2.0f / (far - near);
    ortho[11] = 0.0f;

    ortho[12] = -(right + left) / (right - left);
    ortho[13] = -(top + bottom) / (top - bottom);
    ortho[14] = -(far + near) / (far - near);
    ortho[15] = 1.0f;


    // Clear the screen with a color    
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glViewport(0, 0, viewportWidth, viewportHeight);
    glUniform2f(uViewportSize, viewportWidth, viewportHeight);


    //GViewDef* viewDef = (GViewDef*) view;
    
    //for (int i = 0; i < NUM_VIEWS; i++) {
        GViewInfo viewInfo = (GViewInfo) { 
            (GRect) { 0, 0, 100, 100 },
            (GColor) { 255.0, 0.0, 0.0, 255.0 }
        };

        GView rootView = GView_Alloc(viewInfo);

        GWebGLRenderer_InitForView(rootView);

        GViewDef* viewDef = (GViewDef*) rootView;

        glBindFramebuffer(GL_FRAMEBUFFER, viewDef->framebuffer);
        glViewport(0, 0, viewDef->frame.width, viewDef->frame.height);
        glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUniform2f(uPos, (float)viewDef->frame.x, (float)viewDef->frame.y);
        glUniform2f(uSize, (float)viewDef->frame.width, (float)viewDef->frame.height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, viewDef->texture);
        glUniform1i(uTexture, 0);
        
        glViewport(0, 0, viewportWidth, viewportHeight);        
        glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, ortho);
        glUniform2f(uViewportSize, (float)viewportWidth, (float)viewportHeight);
        printf("SET VIEWPORT SIZE TO %f %f\n", (float)viewportWidth, (float)viewportHeight);
        glUniform2f(uPos, (float)viewDef->frame.x, (float)viewDef->frame.y);
        glUniform2f(uSize, (float)viewDef->frame.width, (float)viewDef->frame.height);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glDrawElements(GL_TRIANGLE_STRIP, , GL_UNSIGNED_INT, 0);

        GView_Free(rootView);

    //}



    
    clock_t now = clock();

    double frameTime = ((double) (now - then));

    GLog(INFO, "Completed WebGL Render in %f us", frameTime);
    
}

void GWebGLRenderer_InitForView(GView view) {
    GViewDef* viewDef = (GViewDef*) view;

    glGenFramebuffers(1, &viewDef->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, viewDef->framebuffer);

    glGenTextures(1, &viewDef->texture);
    glBindTexture(GL_TEXTURE_2D, viewDef->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewDef->frame.width, viewDef->frame.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewDef->texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GWebGLRenderer_SetViewportSize(double width, double height) {
    viewportWidth = width;
    viewportHeight = height;
}