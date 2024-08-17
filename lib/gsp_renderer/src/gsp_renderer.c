#include <gsp_renderer/gsp_renderer.h>

#include <gsp_debug/gsp_debug.h>
#include <gsp_list/gsp_list.h>

#include "glad.h"

#include <stdio.h>
#include <stdlib.h>

const char* surface_vs = "                      \
#version 330 core\n                             \
layout (location = 0) in vec3 aPos;             \
                                                \
void main() {                                   \
	gl_Position = vec4(aPos, 1.0);              \
}                                               \
";

const char* surface_fs = "                      \
#version 330 core\n                             \
out vec4 FragColor;                             \
                                                \
void main()                                     \
{                                               \
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);    \
}                                               \
";



typedef struct {
    grenderer_context_t context;
    uint64_t surface_program;
    uint64_t vao;
} grender_state_t;

static grenderer_context_t current_context = NULL;
static grender_state_t* current_state = NULL;
static glist_t states = NULL;

void gsp_renderer_set_context(grenderer_context_t context) {

    if (NULL == states) {
        if (!gladLoadGL()) {
            gsp_debug_log(FAIL, "Failed to start GLAD");
            return;
        }
        
        gsp_debug_log(INFO, "Started OpenGL v%d.%d", GLVersion.major, GLVersion.minor);

        states = gsp_list_create_list();

    }

    size_t state_count = gsp_list_get_node_count(states);

    for (size_t i = 0; i < state_count; i++) {
        grender_state_t* state = (grender_state_t*)gsp_list_get_data_at_index(states, i);
        
        if (NULL != state && context == state->context) {
            // activate existing context
            return;
        }
    }

    grender_state_t* state = (grender_state_t*)malloc(sizeof(grender_state_t));

    if (NULL != state) {
        
        gnode_t state_node = gsp_list_create_node(states);

        gsp_list_set_node_data(states, state_node, state);   

        //activate new context
        current_state = state;

        state->context = context;

        int compile_success;
        char log_buffer[1024];

        uint32_t surface_vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(surface_vertex, 1, &surface_vs, NULL);
        glCompileShader(surface_vertex);

        glGetShaderiv(surface_vertex, GL_COMPILE_STATUS, &compile_success);
        if (!compile_success) {
            glGetShaderInfoLog(surface_vertex, 1024, NULL, log_buffer);
            gsp_debug_log(FAIL, "Failed to compile vertex shader:\n%s", log_buffer);
        }

        uint32_t surface_fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(surface_fragment, 1, &surface_fs, NULL);
        glCompileShader(surface_fragment);

        glGetShaderiv(surface_fragment, GL_COMPILE_STATUS, &compile_success);
        if (!compile_success) {
            glGetShaderInfoLog(surface_fragment, 1024, NULL, log_buffer);
            gsp_debug_log(FAIL, "Failed to compile fragment shader:\n%s", log_buffer);
            return;
        }

        uint32_t surface_program = glCreateProgram();
        glAttachShader(surface_program, surface_vertex);
        glAttachShader(surface_program, surface_fragment);
        glLinkProgram(surface_program);

        glGetProgramiv(surface_program, GL_LINK_STATUS, &compile_success);
        if (!compile_success) {
            glGetProgramInfoLog(surface_program, 1024, NULL, log_buffer);
            gsp_debug_log(FAIL, "Failed to link shader program:\n%s", log_buffer);
            return;
        }

        state->surface_program = surface_program;

        glDeleteShader(surface_vertex);
        glDeleteShader(surface_fragment);

        gsp_debug_log(INFO, "Successfully compiled and linked shaders");

        GLint maxTextureUnits, maxCombinedTextureUnits, maxArrayTextureLayers;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTextureLayers);

        printf("Max texture units: %d\n", maxTextureUnits);
        printf("Max combined texture units: %d\n", maxCombinedTextureUnits);
        printf("Max array texture layers: %d\n", maxArrayTextureLayers);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
            0.5f, -0.5f, 0.0f, // right 
            0.0f,  0.5f, 0.0f  // top   
        }; 

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0); 

        state->vao = VAO;

    }
}

// destroy all objects created with context
void gsp_renderer_cleanup_context(grenderer_context_t context) {
    printf("cleanup context\n");
}

void gsp_renderer_clear(gcolor_t color) {
    
    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    glClearColor(color.red, color.green, color.blue, color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    printf("use program %u %u\n", current_state->surface_program, current_state->vao);
    glUseProgram(current_state->surface_program); 
    glBindVertexArray(current_state->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}