#include <gsp_renderer/gsp_renderer.h>

#include <gsp_debug/gsp_debug.h>
#include <gsp_list/gsp_list.h>

#include "glad.h"

#include <stdio.h>
#include <stdlib.h>

const char* surface_vs = "                      \
#version 330 core\n                             \
layout (location = 0) in vec3 aPos;             \
layout (location = 1) in vec3 aColor;           \
layout (location = 2) in vec2 aTexCoord;        \
                                                \
out vec3 ourColor;                              \
out vec2 TexCoord;                              \
                                                \
void main() {                                   \
	gl_Position = vec4(aPos, 1.0);              \
	ourColor = aColor;                          \
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);  \
}                                               \
";

const char* surface_fs = "                      \
#version 330 core\n                             \
out vec4 FragColor;                             \
                                                \
in vec3 ourColor;                               \
in vec2 TexCoord;                               \
                                                \
// texture sampler                              \
uniform sampler2D texture1;                     \
                                                \
void main()                                     \
{                                               \
	FragColor = texture(texture1, TexCoord);    \
}                                               \
";

static grenderer_context_t current_context = NULL;

typedef struct {
    grenderer_context_t context;
    uint32_t surface_program;
} grender_state_t;

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

        glDeleteShader(surface_vertex);
        glDeleteShader(surface_fragment);

        gsp_debug_log(INFO, "Successfully compiled and linked shaders");


    }
}

// destroy all objects created with context
void gsp_renderer_cleanup_context(grenderer_context_t context) {
    printf("cleanup context\n");
}

void gsp_renderer_clear(gcolor_t color) {
    glClearColor(color.red, color.green, color.blue, color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}