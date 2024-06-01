#include "GSPCore2/Basic/GWindow.h"

GWindow GWindow_Init(GWindowInfo info) {

    GWindowDef* window = calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

    printf("INIT WINDOW\n");

    window->info = info;

    @autoreleasepool {
        NSRect frame = NSMakeRect(0, 0, info.size.width, info.size.height);
        window->cocoaWindow = [[CocoaWindow alloc] initWithContentRect:frame
                                              styleMask:(NSWindowStyleMaskTitled |
                                                         NSWindowStyleMaskClosable |
                                                         NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
        printf("INIT WINDOW 2\n");

        window->cocoaWindow.window = window;

        NSString* titleString = [NSString stringWithUTF8String:info.title];
        [window->cocoaWindow setTitle:titleString];

        printf("INIT WINDOW 3\n");


        window->cocoaMetalView = [[CocoaMetalView alloc] initWithFrame:frame device:MTLCreateSystemDefaultDevice()];

        if (window->cocoaMetalView == nil) {
            printf("FAILED TO CREATE COCOA VIEW\n");
        } else {
            printf("COCOA VIEW OK\n");
            window->cocoaMetalView.mywindow = window;

        }


        printf("INIT WINDOW 4\n");

                
        window->renderer = GRenderer_InitForWindow(window);
  printf("GOT TO HERE 1\n");

        [window->cocoaWindow setContentView:window->cocoaMetalView];
        

        printf("GOT TO HERE 2\n");

        [window->cocoaWindow center];
        [window->cocoaWindow makeKeyAndOrderFront:nil];     
        
        

    }


    

    
    

    return window;
}

void GWindow_Free(GWindow window) {
    free(window);
}

void GWindow_SetController(GWindow window, GWindowController controller) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->controller = controller;

    @autoreleasepool {
        if (controller != NULL) {
            GWindowControllerDef* controllerDef = (GWindowControllerDef*)controller;
            [windowDef->cocoaWindow setDelegate:controllerDef->cocoaWindowDelegate];
        }
    }
}

@implementation CocoaWindow


@end

@implementation CocoaMetalView

@synthesize mywindow;

- (instancetype)initWithFrame:(NSRect)frameRect device:(id<MTLDevice>)device {
    self = [super initWithFrame:frameRect device:device];

    if (self) {
        // Configure the Metal view
        self.clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 1.0); // Clear color is light gray
        self.paused = true;
        self.enableSetNeedsDisplay = true;
    }

    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    printf("DRAW\n");

    GWindowDef* windowDef = (GWindowDef*)self.mywindow;
    GRendererDef* renderer = (GRendererDef*)windowDef->renderer;

    printf("RENDERER %lu\n", renderer);

    printf("DRAW 2\n");

    id<MTLCommandBuffer> onscreenCommandBuffer = [self.device newCommandQueue].commandBuffer;

    MTLRenderPassDescriptor* onscreenDescriptor = [self currentRenderPassDescriptor];

    // If there's a valid render pass descriptor, use it to render to the current drawable.
    if(onscreenDescriptor != nil) {
        id<MTLRenderCommandEncoder> onscreenCommandEncoder = [onscreenCommandBuffer renderCommandEncoderWithDescriptor:onscreenDescriptor];
        if (onscreenCommandEncoder != nil) { 

            printf("DRAW 3\n");

            [onscreenCommandEncoder setRenderPipelineState: renderer->mtlPipelineState];
            
            printf("DRAW 4\n");
            // Set vertex buffer
            [onscreenCommandEncoder setVertexBuffer:renderer->mtlVertexBuffer offset:0 atIndex:0];

            printf("DRAW 5\n");
            // Set texture
            [onscreenCommandEncoder setFragmentTexture:renderer->mtlTexture atIndex:0];

            printf("DRAW 6\n");
            // Draw call
            [onscreenCommandEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];

            printf("DRAW 7\n");
            [onscreenCommandEncoder endEncoding];
        }


        printf("DRAW 8\n");
        // END encoding your onscreen render pass.


        // Register the drawable's presentation.
        
    }


    // Finalize your onscreen CPU work and commit the command buffer to a GPU.
    [onscreenCommandBuffer presentDrawable:self.currentDrawable];
    [onscreenCommandBuffer commit];

    // Perform rendering here
    // You'll implement your Metal rendering code in this method
}

@end