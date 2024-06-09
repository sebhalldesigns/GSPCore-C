#include "GSPCore/Graphics/GRenderer.h"
#include "GSPCore/GSPCore.h"

float rectVertices[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

float vectorVertices[] = {
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, 1.0f
};

unsigned int vectorIndices[] = {
    0, 1, 3,
    1, 2, 3
};

static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;
static GLuint textureShader;
static GLuint rectVertexArray;
static GLint uPos, uSize, uViewportSize, uProjectionMatrix, uTexture;

static GLuint vectorShader;
static GLuint vectorVertexArray;
static GLint vectorUSize, vectorUNumEntries;
static GLuint vectorUniformBuffer;

typedef struct {
    uint32_t type;
    float padding[3];
    float color[4];
    float posAndStyle[4];
    float paramsA[4];
    float paramsB[4];
    
} VectorGeometry;

VectorGeometry geometry[64];

// a window renderer renders all subviews
GRenderer GRenderer_Init() {

    // MARK: SET UP WEBGL
    if (context == 0) {
        GLog(INFO, "Setting up WebGL");

        EmscriptenWebGLContextAttributes attrs;
        emscripten_webgl_init_context_attributes(&attrs);
        attrs.majorVersion = 2; // Request a WebGL2 context
        attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;

        context = emscripten_webgl_create_context("#canvas", &attrs);
        if (context <= 0) {
            GLog(FAIL, "Failed to make WebGL2 context!");
            return NULL;
        }

        emscripten_webgl_make_context_current(context);

        GLog(INFO, "Created WebGL2 Context");

        // set up SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            GLog(FAIL, "Could not initialize SDL: %s\n", SDL_GetError());
            return NULL;
        }

        if (TTF_Init() == -1) {
            GLog(FAIL, "Could not initialize TTF: %s\n", TTF_GetError());
            SDL_Quit();
            return NULL;
        }

        GLuint vectorVertexShader = WebGLCompileShader(GL_VERTEX_SHADER, "../resources/wasm/shaders/vector/GeneralVectorVert.glsl");
        GLuint vectorFragmentShader = WebGLCompileShader(GL_FRAGMENT_SHADER, "../resources/wasm/shaders/vector/GeneralVectorFrag.glsl");
        vectorShader = WebGLCreateProgram(vectorVertexShader, vectorFragmentShader);

        glDeleteShader(vectorVertexShader);
        glDeleteShader(vectorFragmentShader);

        if (vectorShader == 0) {
            GLog(FAIL, "Failed to compile vector shader");
            return NULL;
        }

        GLog(INFO, "Compiled vector shader");

        vectorUSize = glGetUniformLocation(vectorShader, "uSize");
        vectorUNumEntries = glGetUniformLocation(vectorShader, "numEntries");


        glUseProgram(vectorShader);

        glGenVertexArrays(1, &vectorVertexArray);
        glBindVertexArray(vectorVertexArray);

        GLuint vectorVertexBuffer;
        glGenBuffers(1, &vectorVertexBuffer);

        GLuint vectorElementBuffer;
        glGenBuffers(1, &vectorElementBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, vectorVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vectorVertices), vectorVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vectorElementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vectorIndices), vectorIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        GLuint textureVertexShader = WebGLCompileShader(GL_VERTEX_SHADER, "../resources/wasm/shaders/compositor/RectangularCompositorVert.glsl");
        GLuint textureFragmentShader = WebGLCompileShader(GL_FRAGMENT_SHADER, "../resources/wasm/shaders/compositor/RectangularCompositorFrag.glsl");
        textureShader = WebGLCreateProgram(textureVertexShader, textureFragmentShader);

        glDeleteShader(textureVertexShader);
        glDeleteShader(textureFragmentShader);

        if (textureShader == 0) {
            GLog(FAIL, "Failed to compile texture shader");
            return NULL;
        }

        GLog(INFO, "Compiled texture shader");

        glUseProgram(textureShader);
        
        glGenVertexArrays(1, &rectVertexArray);
        glBindVertexArray(rectVertexArray);

        GLuint rectVertexBuffer;
        glGenBuffers(1, &rectVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, rectVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        uPos = glGetUniformLocation(textureShader, "uPos");
        uSize = glGetUniformLocation(textureShader, "uSize");
        uViewportSize = glGetUniformLocation(textureShader, "uViewportSize");
        uProjectionMatrix = glGetUniformLocation(textureShader, "uProjectionMatrix");
        uTexture = glGetUniformLocation(textureShader, "uTexture");

        GLog(INFO, "WebGL2 setup complete");

    }

    // MARK: INIT VIEW

    GRendererDef* rendererDef = calloc(1, sizeof(GRendererDef));

    if (rendererDef == NULL) {
        GLog(FAIL, "Failed to allocate GRenderer!");
        return NULL;
    }

    glGenFramebuffers(1, &rendererDef->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, rendererDef->framebuffer);

    glGenTextures(1, &rendererDef->renderedTexture);
    glBindTexture(GL_TEXTURE_2D, rendererDef->renderedTexture);

    // set up framebuffer texture as a 1x1 image temporarily
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    rendererDef->textureSize = (GSize) { 1, 1};
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendererDef->renderedTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete\n");
        free(rendererDef);
        glDeleteFramebuffers(1, &rendererDef->framebuffer);
        return NULL;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return rendererDef;
}


void GRenderer_Free(GRenderer renderer) {

}

void GRenderer_RenderSelf(GRenderer renderer) {

    if (renderer == NULL) {
        return;
    }

    GRendererDef* rendererDef = (GRendererDef*)renderer;

    if (rendererDef->view == NULL) {
        GLog(FAIL, "No view set for renderer!");
        return;
    }

    GLog(INFO, "Starting WebGL view render...");
    clock_t then = clock();

    glBindFramebuffer(GL_FRAMEBUFFER, rendererDef->framebuffer);

    GViewDef* viewDef = (GViewDef*)rendererDef->view;
    if (viewDef->frame.size.width != rendererDef->textureSize.width || viewDef->frame.size.height != rendererDef->textureSize.height) {
        // remake texture as view size has changed
        glBindTexture(GL_TEXTURE_2D, rendererDef->renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)viewDef->frame.size.width, (int)viewDef->frame.size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        rendererDef->textureSize = (GSize) { viewDef->frame.size.width, viewDef->frame.size.height};
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendererDef->renderedTexture, 0);

        GLog(INFO, "Resized framebuffer texture");
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClearColor(viewDef->backgroundColor.red, viewDef->backgroundColor.green, viewDef->backgroundColor.blue, viewDef->backgroundColor.alpha); 
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, viewDef->frame.size.width, viewDef->frame.size.height);        
    printf("SET BACKGROUND TO %f %f \n", viewDef->frame.size.width, viewDef->frame.size.height);


    // RENDER VIEW HERE
    glUseProgram(vectorShader);
    glUniform2f(vectorUSize, (float)viewDef->frame.size.width, (float)viewDef->frame.size.height);

    glUniform1i(vectorUNumEntries, 1);
    /*
    for (int i = 0; i < 64; i++) {
        vectorData[i].position[0] = ((float)rand()/(float)(RAND_MAX)) * viewDef->frame.size.width;
        vectorData[i].position[1] = ((  float)rand()/(float)(RAND_MAX)) * viewDef->frame.size.height;
    }
 

    vectorData[0].position[0] = viewDef->lastMouseLocation.x;
    vectorData[0].position[1] = viewDef->lastMouseLocation.y;
*/

    geometry[0].type = 1;
    geometry[0].posAndStyle[0] = viewDef->lastMouseLocation.x;
    geometry[0].posAndStyle[1] = viewDef->lastMouseLocation.y;
    geometry[0].paramsA[0] = 10.0;
    geometry[0].posAndStyle[2] = 1.0;
    
    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(VectorGeometry) * 64, geometry, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

    GLuint blockIndex = glGetUniformBlockIndex(vectorShader, "VectorGeometryBlock");
    glUniformBlockBinding(vectorShader, blockIndex, 0);

    
    glBindVertexArray(vectorVertexArray);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    clock_t now = clock();
    double frameTime = ((double) (now - then));
    GLog(INFO, "Completed WebGL view render in %f us", frameTime);
}

void GRenderer_RenderRoot(GRenderer renderer) {

    if (renderer == NULL) {
        return;
    }

    GRendererDef* rendererDef = (GRendererDef*)renderer;

    GLog(INFO, "Starting WebGL root render...");
    clock_t then = clock();
    

    // Clear the screen with a color    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    if (rendererDef->view == NULL) {
        GLog(FAIL, "No view set for renderer!");
        return;
    }
    


    GViewDef* viewDef = (GViewDef*)rendererDef->view;

    int viewportWidth = viewDef->frame.size.width;
    int viewportHeight = viewDef->frame.size.height;
    glViewport(0, 0, viewportWidth, viewportHeight);        
    glUniform2f(uViewportSize, (float)viewportWidth, (float)viewportHeight);
    printf("SET VIEWPORT SIZE TO %f %f\n", (float)viewportWidth, (float)viewportHeight);


    size_t subviewCount = GVector_Size(viewDef->subviews);

    for (size_t i = 0; i < subviewCount; i++) {

        GViewDef* subview = GVector_Get(viewDef->subviews, i);
        
        if (subview->renderer != NULL) {

            GRendererDef* subviewRenderer = (GRendererDef*)subview->renderer;

            glUseProgram(textureShader);

            glUniform2f(uPos, (float)subview->frame.origin.x, (float)subview->frame.origin.y);
            glUniform2f(uSize, (float)subview->frame.size.width, (float)subview->frame.size.height);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, subviewRenderer->renderedTexture);
            glUniform1i(uTexture, 0);

            glBindVertexArray(rectVertexArray);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        }


    }


    clock_t now = clock();
    double frameTime = ((double) (now - then));
    GLog(INFO, "Completed WebGL root render in %f us", frameTime);
}


GLuint WebGLCompileShader(GLenum type, const char* path) {

    char * buffer = 0;
    long length;
    FILE * f = fopen(path, "rb");

    if (f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length + 1);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose (f);
        buffer[length] = '\0';
    }

    if (!buffer) {
        GLog(FAIL, "Failed to read shader file %s\n", path);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &buffer, NULL);
    glCompileShader(shader);

    free(buffer);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        GLog(FAIL, "Failed to compile WebGL shader\n%s\n", infoLog);
    }

    return shader;
}

GLuint WebGLCreateProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        GLog(FAIL, "Failed to create WebGL shader program! \n%s\n", infoLog);
        return 0;
    }

    return shaderProgram;
}