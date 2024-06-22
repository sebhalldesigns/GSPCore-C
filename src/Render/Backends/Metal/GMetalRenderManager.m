#include <GSPCore/GSPCore.h>
#include <GSPCore/Render/Backends/Metal/GMetalRenderManager.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool GMetalRenderManager_Init() {

    GLog(INFO, "Started GRenderManager on Metal backend");



    return true;
}
bool GMetalRenderManager_SetupWindow(GWindow* window) {

    GLog(INFO, "Setting up Metal for window");

    window->platformHandles.mtlDevice = [window->platformHandles.metalView device];

    char* vertexShader = GMetalRenderManager_LoadShader("../resources/metal/shaders/vertex.metal");
    if (vertexShader == NULL) {
        return false;
    }

    NSError *error = nil;

    NSString* vertexSource = [NSString stringWithUTF8String:vertexShader];

    id<MTLLibrary> vertexLibrary = [window->platformHandles.mtlDevice newLibraryWithSource:vertexSource options:nil error:&error];
    if (!vertexLibrary) {
        GLog(WARNING, "Failed to compile vertex shader: \n%s", [[error localizedDescription] UTF8String]);
        return false;
    }

    id<MTLFunction> vertexFunction = [vertexLibrary newFunctionWithName:@"vertexShader"];

    char* fragmentShader = GMetalRenderManager_LoadShader("../resources/metal/shaders/fragment.metal");
    if (fragmentShader == NULL) {
        return false;
    }

    NSString* fragmentSource = [NSString stringWithUTF8String:fragmentShader];

    id<MTLLibrary> fragmentLibrary = [window->platformHandles.mtlDevice newLibraryWithSource:fragmentSource options:nil error:&error];
    if (!fragmentLibrary) {
        GLog(WARNING, "Failed to compile fragment shader: \n%s", [[error localizedDescription] UTF8String]);
        return false;
    }

    id<MTLFunction> fragmentFunction = [fragmentLibrary newFunctionWithName:@"fragmentShader"];

     // Create render pipeline descriptor
    MTLRenderPipelineDescriptor *pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat = window->platformHandles.metalView.colorPixelFormat;

    // Create render pipeline state
    window->platformHandles.mtlPipelineState = [window->platformHandles.mtlDevice newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                                                     error:&error];
    if (!window->platformHandles.mtlPipelineState) {
        GLog(WARNING, "Failed to create render pipeline state: \n%s", [[error localizedDescription] UTF8String]);
        return false;
    }

    // Create vertex buffer (for rendering quad)
    static const float vertices[] = {
        -1.0, -1.0, 0.0, 1.0,
        1.0, -1.0, 0.0, 1.0,
        -1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0
    };

    window->platformHandles.mtlVertexBuffer = [window->platformHandles.mtlDevice newBufferWithBytes:vertices
                                                 length:sizeof(vertices)
                                                options:MTLResourceStorageModeShared];

    window->platformHandles.image = GResourceManager_LoadImage("../resources/images/image.png");

    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    textureDescriptor.width = window->platformHandles.image.width;
    textureDescriptor.height = window->platformHandles.image.height;
    textureDescriptor.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

    // Create Metal texture
    window->platformHandles.mtlTexture = [window->platformHandles.mtlDevice newTextureWithDescriptor:textureDescriptor];

    // Copy pixel data from Core Graphics image to Metal texture
    MTLRegion region = {
        {0, 0, 0},
        {textureDescriptor.width, textureDescriptor.height, 1}
    };

    [window->platformHandles.mtlTexture replaceRegion:region mipmapLevel:0 withBytes:window->platformHandles.image.rawData bytesPerRow:4 * textureDescriptor.width];

    return true;
}
void GMetalRenderManager_RenderWindow(GWindow* window) {
    
    time_t start = clock();
    id<MTLCommandBuffer> onscreenCommandBuffer = [window->platformHandles.metalView.commandQueue commandBuffer];
    MTLRenderPassDescriptor* onscreenDescriptor = [window->platformHandles.metalView currentRenderPassDescriptor];
    
    if(onscreenDescriptor != nil) {
        id<MTLRenderCommandEncoder> onscreenCommandEncoder = [onscreenCommandBuffer renderCommandEncoderWithDescriptor:onscreenDescriptor];
        if (onscreenCommandEncoder != nil) { 

            printf("DRAW 3\n");

            [onscreenCommandEncoder setRenderPipelineState:window->platformHandles.mtlPipelineState];
            
            printf("DRAW 4\n");
            // Set vertex buffer
            [onscreenCommandEncoder setVertexBuffer:window->platformHandles.mtlVertexBuffer offset:0 atIndex:0];

            printf("DRAW 5\n");
            // Set texture
            [onscreenCommandEncoder setFragmentTexture:window->platformHandles.mtlTexture atIndex:0];

            printf("DRAW 6\n");
            // Draw call
            [onscreenCommandEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];

            printf("DRAW 7\n");
            [onscreenCommandEncoder endEncoding];
        }
    }

    [onscreenCommandBuffer presentDrawable:window->platformHandles.metalView.currentDrawable];
    [onscreenCommandBuffer commit];

    //[onscreenCommandBuffer waitUntilCompleted]; 

    GLog(INFO, "Render complete in %lluus", clock() - start);

}

char* GMetalRenderManager_LoadShader(char* path) {

    FILE* fileHandle = fopen(path, "r");

    if (fileHandle == NULL) {
        GLog(WARNING, "Failed to open shader file \'%s\'", path);
        return NULL;
    }

    long numbytes;
    fseek(fileHandle, 0L, SEEK_END);
    numbytes = ftell(fileHandle);

    fseek(fileHandle, 0L, SEEK_SET);

    char* buffer = (char*)calloc(numbytes, sizeof(char));	

    if (buffer == NULL) {
        GLog(WARNING, "Failed to allocate for shader file \'%s\'", path);
        return NULL;
    }

    fread(buffer, sizeof(char), numbytes, fileHandle);
    fclose(fileHandle);
    
    return buffer;
}