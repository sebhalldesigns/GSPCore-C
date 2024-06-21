#include <GSPCore/GSPCore.h>
#include <GSPCore/Resource/Providers/Cocoa/GCocoaResourceManager.h>

#include <string.h>

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>



bool GCocoaResourceManager_Init() {

    GLog(INFO, "Started GResourceManager on Cocoa backend");


    return true;
}


// note: this is probably a very slow implementation but it's the first version
// image loading will be inherantly slow regardless.
GRawImage GCocoaResourceManager_LoadImage(const char* path) {

    NSString* pathString = [NSString stringWithUTF8String:path];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile: pathString];
    
    if (!image) {
        GLog(WARNING, "Failed to load image at path %s", path);
        return (GRawImage){NULL, 0, 0};
    }

    GLog(INFO, "Loaded image resource from %s", path);

    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CGContextRef ctx = CGBitmapContextCreate(NULL,
                                             image.size.width,
                                             image.size.height,
                                             8,
                                             0,
                                             colorSpace,
                                             kCGImageAlphaPremultipliedLast);
    
    NSGraphicsContext* gctx = [NSGraphicsContext graphicsContextWithCGContext:ctx flipped:NO];

    [NSGraphicsContext setCurrentContext:gctx];

    NSRect imageRect = NSMakeRect(0, 0, image.size.width, image.size.height);
    [image drawInRect:imageRect];

    size_t width = CGBitmapContextGetWidth(ctx);
    size_t height = CGBitmapContextGetHeight(ctx);

    uint32_t* pixel = (uint32_t*)CGBitmapContextGetData(ctx);

    GRawImage rawImage;
    rawImage.width = width;
    rawImage.height = height;
    rawImage.rawData = calloc(width*height, sizeof(uint32_t));

    if (rawImage.rawData == NULL) {
        return (GRawImage){NULL, 0, 0};
    }

    memcpy(rawImage.rawData, pixel, sizeof(uint32_t) * width * height);

    CGContextRelease(ctx);
    CGColorSpaceRelease(colorSpace);

    return rawImage;

}


void GCocoaResourceManager_SaveImage(GRawImage image, char* path) {

    // Create a color space
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    // Create a bitmap context from the raw pixel data
    CGContextRef context = CGBitmapContextCreate(image.rawData, image.width, image.height, 8, image.width * 4 + 1, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);

    if (!context) {
        NSLog(@"Failed to create bitmap context");
        return;
    }

    // Create a CGImage from the bitmap context
    CGImageRef imageRef = CGBitmapContextCreateImage(context);
    CGContextRelease(context);

    if (!imageRef) {
        NSLog(@"Failed to create CGImage");
        return;
    }

    // Create an NSBitmapImageRep from the CGImage
    NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:imageRef];
    CGImageRelease(imageRef);

    // Convert the NSBitmapImageRep to PNG data
    NSData *pngData = [bitmapRep representationUsingType:NSBitmapImageFileTypePNG properties:@{}];


    NSString* pathString = [NSString stringWithUTF8String:path];
    [pngData writeToFile:pathString atomically:YES];
}


GSize GCocoaResourceManager_MeasureText(const char* text) {

}

GRawImage GCocoaResourceManager_LoadText(const char* text) {

}