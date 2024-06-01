
#include "GSPCore2/Graphics/GRenderer.h"

const char* vertexShaderSource = "\n\
#include <metal_stdlib>\n\
using namespace metal;\n\
\n\
struct Vertex {\n\
    float4 position [[position]];\n\
    float2 texCoord;\n\
};\n\
\n\
vertex Vertex vertexShader(uint vertexID [[vertex_id]]) {\n\
    float2 positions[4] = {\n\
        float2(-1.0, -1.0),\n\
        float2(1.0, -1.0),\n\
        float2(-1.0, 1.0),\n\
        float2(1.0, 1.0)\n\
    };\n\
    float2 texCoords[4] = {\n\
        float2(0.0, 1.0),\n\
        float2(1.0, 1.0),\n\
        float2(0.0, 0.0),\n\
        float2(1.0, 0.0)\n\
    };\n\
    \n\
    Vertex outVertex;\n\
    outVertex.position = float4(positions[vertexID], 0.0, 1.0);\n\
    outVertex.texCoord = texCoords[vertexID];\n\
    return outVertex;\n\
}\n\
";

const char* fragmentShaderSource = "\n\
#include <metal_stdlib>\n\
using namespace metal;\n\
\n\
struct Vertex {\n\
    float4 position [[position]];\n\
    float2 texCoord;\n\
};\n\
\n\
fragment float4 fragmentShader(Vertex in [[stage_in]], texture2d<float> tex [[texture(0)]]) {\n\
    //return float4(0.0, 0.0, 1.0, 1.0); // Clear color: blue\n\
    constexpr sampler texSampler (mag_filter::linear, min_filter::linear);\n\
    return tex.sample(texSampler, in.texCoord);\n\
}\n\
";



GRenderer GRenderer_InitForWindow(GWindow window) {

    GRendererDef* renderer = calloc(1, sizeof(GRendererDef));

    printf("RENDERER ALLOC %lu\n", renderer);


    printf("RENDER INIT\n");

    if (renderer == NULL || window == NULL) {
        return NULL;
    }

    NSError *error = nil;

    GWindowDef* windowDef = (GWindowDef*)window;

    renderer->mtlDevice = windowDef->cocoaMetalView.device;

    printf("RENDER INIT 2\n");

    // Compile vertex shader
    NSString *vertexShaderString = [NSString stringWithUTF8String:vertexShaderSource];
    id<MTLLibrary> vertexLibrary = [renderer->mtlDevice newLibraryWithSource:vertexShaderString options:nil error:&error];
    if (!vertexLibrary) {
        printf("Failed to compile vertex shader\n");
        NSLog(@"Failed to compile vertex shader: %@", error);
        return NULL;
    }

    id<MTLFunction> vertexFunction = [vertexLibrary newFunctionWithName:@"vertexShader"];
    
    // Compile fragment shader
    NSString *fragmentShaderString = [NSString stringWithUTF8String:fragmentShaderSource];
    id<MTLLibrary> fragmentLibrary = [renderer->mtlDevice newLibraryWithSource:fragmentShaderString options:nil error:&error];
    if (!fragmentLibrary) {
        //NSLog(@"Failed to compile fragment shader: %@", error);
        printf("Failed to compile fragement shader\n");
        return NULL;
    }
    id<MTLFunction> fragmentFunction = [fragmentLibrary newFunctionWithName:@"fragmentShader"];
    

    // Create render pipeline descriptor
    MTLRenderPipelineDescriptor *pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat = windowDef->cocoaMetalView.colorPixelFormat;

    // Create render pipeline state
    renderer->mtlPipelineState = [renderer->mtlDevice newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                                                     error:&error];
    if (!renderer->mtlPipelineState) {
        printf("Failed to create render pipeline state\n");
        return NULL;
    }

 
    // Create vertex buffer (for rendering quad)
    static const float vertices[] = {
        -1.0, -1.0, 0.0, 1.0,
        1.0, -1.0, 0.0, 1.0,
        -1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0
    };
    renderer->mtlVertexBuffer = [renderer->mtlDevice newBufferWithBytes:vertices
                                                 length:sizeof(vertices)
                                                options:MTLResourceStorageModeShared];

    // Create a Core Text attributed string
    NSString *textString = @"Hello, Metal!";
    NSDictionary *attributes = @{
        NSFontAttributeName: [NSFont systemFontOfSize:24],
      
        NSForegroundColorAttributeName: [NSColor whiteColor]
    };
    NSAttributedString *attributedString = [[NSAttributedString alloc] initWithString:textString
                                                                           attributes:attributes];

    // Create a Core Text frame setter
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)attributedString);

    // Create a Core Graphics context
    CGContextRef context = CGBitmapContextCreate(NULL, 256, 256, 8, 0, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);

    // Create a Core Text frame
    CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, [attributedString length]), CGPathCreateWithRect(CGRectMake(0, 0, 256, 256), NULL), NULL);

    // Draw the text onto the Core Graphics context
    CTFrameDraw(frame, context);

    // Create a Metal texture from the Core Graphics context
    CGImageRef imageRef = CGBitmapContextCreateImage(context);
    //renderer->mtlTexture = [windowDef->cocoaMetalView textureFromCGImage:imageRef];

    // Create Metal texture descriptor
    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    textureDescriptor.width = CGImageGetWidth(imageRef);
    textureDescriptor.height = CGImageGetHeight(imageRef);
    textureDescriptor.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;

    // Create Metal texture
    renderer->mtlTexture = [renderer->mtlDevice newTextureWithDescriptor:textureDescriptor];

    // Copy pixel data from Core Graphics image to Metal texture
    MTLRegion region = {
        {0, 0, 0},
        {textureDescriptor.width, textureDescriptor.height, 1}
    };
    Byte *imageData = (Byte *)calloc(textureDescriptor.width * textureDescriptor.height * 4, sizeof(Byte));
    CGContextRef context2 = CGBitmapContextCreate(imageData, textureDescriptor.width, textureDescriptor.height, 8, 4 * textureDescriptor.width, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(context2, CGRectMake(0, 0, textureDescriptor.width, textureDescriptor.height), imageRef);
    CGContextRelease(context2);
    [renderer->mtlTexture replaceRegion:region mipmapLevel:0 withBytes:imageData bytesPerRow:4 * textureDescriptor.width];

    // Free allocated memory
    free(imageData);


    // Release Core Text and Core Graphics objects
    CFRelease(frame);
    CFRelease(framesetter);
    CGContextRelease(context);
    CGImageRelease(imageRef);

    printf("renderer setup ok\n");

    
    
    return renderer;
}


void GRenderer_Free(GRenderer renderer) {

}