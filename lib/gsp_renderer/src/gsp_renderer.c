#include <gsp_renderer/gsp_renderer.h>

#include <gsp_debug/gsp_debug.h>
#include <gsp_containers/gsp_list.h>
#include <gsp_drawing/gsp_drawing_d2d1.h>

#include "glad.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* surface_vs = "                          \
#version 330 core                                   \n\
                                                    \n\
layout (location = 0) in vec2 position;             \n\
layout (location = 1) in vec2 tex_coord;            \n\
                                                    \n\
layout (location = 2) in vec4 instance_transform;   \n\
layout (location = 3) in uint instance_texture;     \n\
                                                    \n\
out vec2 texture_coord;                             \n\
flat out uint texture_id;                           \n\
                                                    \n\
uniform vec2 viewport_size;                         \n\
                                                    \n\
void main() {                                       \n\
    vec2 pos_2d = position * instance_transform.zw  \n\
    + instance_transform.xy;                        \n\
    vec2 pos_ndc = (pos_2d / viewport_size)         \n\
        * 2.0 - 1.0;                                \n\
    pos_ndc.y = -pos_ndc.y;                         \n\
	gl_Position = vec4(pos_ndc, 0.0, 1.0);          \n\
                                                    \n\
    texture_coord = tex_coord;                      \n\
    texture_id = instance_texture;                  \n\
}                                                   \n\
";

const char* surface_fs = "                          \
#version 330 core                                   \n\
                                                    \n\
in vec2 texture_coord;                              \n\
flat in uint texture_id;                            \n\
                                                    \n\
out vec4 FragColor;                                 \n\
                                                    \n\
uniform sampler2D textures[16];                     \n\
                                                    \n\
void main() {                                       \n\
	 if (texture_id == 0u) { \n\
        FragColor = texture(textures[0], texture_coord);\n\
    } else if (texture_id == 1u) {\n\
        FragColor = texture(textures[1], texture_coord);\n\
    } else {\n\
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Draw red for error\n\
    }\n\
}                                                   \n\
";

const float surface_vertices[] = {
    // Positions   // TexCoords
    0.0f, 0.0f,    0.0f, 0.0f,  // Bottom-left
    1.0f, 0.0f,    1.0f, 0.0f,  // Bottom-right
    1.0f, 1.0f,    1.0f, 1.0f,  // Top-right
    0.0f, 1.0f,    0.0f, 1.0f   // Top-left
};

unsigned int surface_indices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};


typedef struct {
    gwindow_t window;
    grenderer_context_t context;
    uint64_t surface_program;
    uint64_t vao;
    uint64_t textures[16];
    uint64_t viewport_size_uniform;
    gsize_t viewport_size;
} grender_state_t;

static grenderer_context_t current_context = NULL;
static grender_state_t* current_state = NULL;
static glist_t states = NULL;

void gsp_renderer_set_context(gwindow_t window, grenderer_context_t context) {

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
        
        glist_node_t state_node = gsp_list_create_node(states);

        gsp_list_set_node_data(states, state_node, state);   

        //activate new context
        current_state = state;

        state->context = context;
        state->window = window;

        int compile_success;
        char log_buffer[1024];

        uint32_t surface_vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(surface_vertex, 1, &surface_vs, NULL);
        glCompileShader(surface_vertex);

        glGetShaderiv(surface_vertex, GL_COMPILE_STATUS, &compile_success);
        if (!compile_success) {
            glGetShaderInfoLog(surface_vertex, 1024, NULL, log_buffer);
            gsp_debug_log(FAIL, "Failed to compile vertex shader:\n%s", log_buffer);
            return;
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

        // Setup VAO, VBO, and EBO
        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        state->vao = VAO;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(surface_vertices), surface_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(surface_indices), surface_indices, GL_STATIC_DRAW);

        // Vertex position
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);


        float instanceData[] = {
            // Instance 1
            10.0f, 10.0f, 500.0f, 500.0f,
            520.0f, 10.0f, 500.0f, 500.0f,
        };

        uint32_t textureIDs[] = { 0u, 1u }; // Assuming texture 0 and texture 1 are used

        int numInstances = 2; 

        glBindVertexArray(VAO);

        // Instance transform attribute (x, y, w, h)
        GLuint instanceVBO;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, numInstances * 4 * sizeof(float), instanceData, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1); // Set the divisor for instanced rendering

        // Instance texture ID attribute
        GLuint textureVBO;
        glGenBuffers(1, &textureVBO);
        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(uint32_t), textureIDs, GL_STATIC_DRAW);

        glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(uint32_t), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1); // Set the divisor for instanced rendering

        /*
        for (int i = 0; i < 16; i++) {
            glGenTextures(1, &state->textures[i]);
        }
    

        int width, height, channels;
        //stbi_set_flip_vertically_on_load(true);
        unsigned char *img = stbi_load("sky.png", &width, &height, &channels, 0);

        if(img == NULL) {
            printf("Error in loading the image\n");
            return;
        }

        printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, state->textures[0]);   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

        // Add these lines
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *img2 = stbi_load("red.png", &width, &height, &channels, 0);

        gbitmap_t bitmap = gsp_drawing_d2d1_render_bitmap(0);

        if(img2 == NULL) {
            printf("Error in loading the image\n");
            return;
        }

        width = bitmap.width;
        height = bitmap.height;


        printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels); 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, state->textures[1]);   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.data);

        // Add these lines
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        */

        glUseProgram(state->surface_program);
        // Set up texture uniforms
        for (int i = 0; i < 16; i++) {
            char uniformName[20];
            snprintf(uniformName, sizeof(uniformName), "textures[%d]", i);
            glUniform1i(glGetUniformLocation(state->surface_program, uniformName), i);
        }

        state->viewport_size_uniform = glGetUniformLocation(state->surface_program, "viewport_size");
    }
}

uint32_t gsp_renderer_allocate_texture() {

    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    uint32_t texture;
    glGenTextures(1, &texture);

    gsp_debug_log(INFO, "Allocated texture %u", texture);

    return texture;
}

void gsp_renderer_upload_file(uint32_t texture, char* file) {

    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    int width, height, channels;
    unsigned char *img = stbi_load(file, &width, &height, &channels, 0);

    if(img == NULL) {
        printf("Error in loading the image\n");
        return;
    }

    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
}

void gsp_renderer_upload_texture(uint32_t texture, int width, int height, char* data) {

    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);   

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gsp_debug_log(INFO, "Uploaded data for texture %u", texture);
}

void gsp_renderer_bind_texture(uint32_t texture, uint32_t id) {

    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    glUseProgram(current_state->surface_program); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void gsp_renderer_buffer_data(uint32_t instances, void* vertices, void* texture_ids) {
    
    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    /*float instanceData[] = {
        // Instance 1
        10.0f, 10.0f, 500.0f, 500.0f,
        520.0f, 10.0f, 500.0f, 500.0f,
    };

    uint32_t textureIDs[] = { 0u, 1u }; // Assuming texture 0 and texture 1 are used*/

    glBindVertexArray(current_state->vao);

    // Instance transform attribute (x, y, w, h)
    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instances * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // Set the divisor for instanced rendering

    // Instance texture ID attribute
    GLuint textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, instances * sizeof(uint32_t), texture_ids, GL_STATIC_DRAW);

    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(uint32_t), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // Set the divisor for instanced rendering

}

void gsp_renderer_draw_instances(uint32_t instances) {
     if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    glUseProgram(current_state->surface_program); 
    glBindVertexArray(current_state->vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances); // 2 instances*/

}

// destroy all objects created with context
void gsp_renderer_cleanup_context(grenderer_context_t context) {
    printf("cleanup context\n");
}

void gsp_renderer_set_viewport(gwindow_t window, grenderer_context_t context, gsize_t size) {
    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    glUseProgram(current_state->surface_program);     
    glViewport(0, 0, (int)size.width, (int)size.height);
    glUniform2f(current_state->viewport_size_uniform, size.width, size.height);

    current_state->viewport_size = size;

        printf("updated size: %f %f\n", size.width, size.height);

}

void gsp_renderer_clear(gcolor_t color) {
    
    if (NULL == current_state) {
        gsp_debug_log(WARNING, "No render context bound!");
        return;    
    }

    glClearColor(color.red, color.green, color.blue, color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    printf("use program %u %u\n", current_state->surface_program, current_state->vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current_state->textures[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, current_state->textures[1]);


    glUseProgram(current_state->surface_program); 
    glUniform2f(current_state->viewport_size_uniform, current_state->viewport_size.width, current_state->viewport_size.height);

    glBindVertexArray(current_state->vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2); // 2 instances*/

}




grenderer_surface_t gsp_renderer_create_surface(gwindow_t window) {


    return NULL;
}

void gsp_renderer_set_surface_frame(gwindow_t window, grenderer_surface_t surface, grect_t frame) {

}

void gsp_renderer_set_surface_background(gwindow_t window, grenderer_surface_t surface, gcolor_t background_color) {

}

void gsp_renderer_upload_surface_texture(gwindow_t window, grenderer_surface_t surface, uint32_t width, uint32_t height, uint8_t* data) {

}

