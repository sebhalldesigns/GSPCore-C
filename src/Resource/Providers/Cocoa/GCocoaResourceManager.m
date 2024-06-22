#include <GSPCore/GSPCore.h>
#include <GSPCore/Resource/Providers/Cocoa/GCocoaResourceManager.h>

#include <string.h>
#include <time.h>

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreText/CoreText.h>


bool GCocoaResourceManager_Init() {

    GLog(INFO, "Started GResourceManager on Cocoa backend");


    return true;
}


// note: this is probably a very slow implementation but it's the first version
// image loading will be inherantly slow regardless.
GRawImage GCocoaResourceManager_LoadImage(const char* path) {

    @autoreleasepool {

        clock_t start = clock();

        NSString* pathString = [NSString stringWithUTF8String:path];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile: pathString];
        
        if (!image) {
            GLog(WARNING, "Failed to load image at path \'%s\'", path);
            return (GRawImage){NULL, 0, 0};
        }


        NSBitmapImageRep* bmp = [[image representations] objectAtIndex:0];
        size_t bits = [bmp bitsPerPixel];


        /*CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
        CGContextRef ctx = CGBitmapContextCreate(NULL,
                                                image.size.width,
                                                image.size.height,
                                                8,
                                                image.size.width * 4,
                                                colorSpace,
                                                kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        
        NSGraphicsContext* gctx = [NSGraphicsContext graphicsContextWithCGContext:ctx flipped:NO];

        [NSGraphicsContext setCurrentContext:gctx];

        NSRect imageRect = NSMakeRect(0, 0, image.size.width, image.size.height);
        [image drawInRect:imageRect fromRect:NSZeroRect operation:NSCompositingOperationCopy fraction:1.0];

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

        CGContextRelease(ctx);*/

        GRawImage rawImage;
        rawImage.width = [bmp pixelsWide];
        rawImage.height = [bmp pixelsHigh];
        rawImage.rawData = calloc(rawImage.width*rawImage.height, sizeof(uint32_t));

        if (rawImage.rawData == NULL) {
            return (GRawImage){NULL, 0, 0};
        }

        memcpy(rawImage.rawData, [bmp bitmapData], sizeof(uint32_t) * rawImage.width * rawImage.height);

        GLog(INFO, "Loaded image resource from \'%s\' in %luus", path, clock() - start);

        return rawImage;

    }    
    
}


void GCocoaResourceManager_SaveImage(GRawImage image, char* path) {

    @autoreleasepool {
        clock_t start = clock();

        // Create a color space
        CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
        
        // Create a bitmap context from the raw pixel data
        CGContextRef context = CGBitmapContextCreate(image.rawData, image.width, image.height, 8, image.width * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
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

        GLog(INFO, "Saved image resource to \'%s\' in %lluus", path, clock() - start);

    }
    
}


GSize GCocoaResourceManager_MeasureText(const char* text, const char* typeface, float size) {
    @autoreleasepool {
        clock_t start = clock();

        NSString* nsText = [NSString stringWithUTF8String:text];
        CFStringRef typefaceRef = CFStringCreateWithCString(NULL, typeface, kCFStringEncodingUTF8);
        CTFontRef font = CTFontCreateWithName(typefaceRef, size, NULL); // Use a default font
        
        NSDictionary* attributes = @{ (id)kCTFontAttributeName: (__bridge id)font };
        NSAttributedString* attributedString = [[NSAttributedString alloc] initWithString:nsText attributes:attributes];
        
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)attributedString);
        CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, nsText.length), NULL, CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX), NULL);
        
        CFRelease(framesetter);
        CFRelease(font);
        
        GSize size = { ceil(suggestedSize.width), ceil(suggestedSize.height) };

        GLog(INFO, "Measured text \'%s\' in %lluus", text, clock() - start);

        return size;
    }
}

GRawImage GCocoaResourceManager_LoadText(const char* text, const char* typeface, float size, GColor color, float maxWidth) {
    @autoreleasepool {

        clock_t start = clock();

        NSString* nsText = [NSString stringWithUTF8String:text];
        CFStringRef typefaceRef = CFStringCreateWithCString(NULL, typeface, kCFStringEncodingUTF8);
        CTFontRef font = CTFontCreateWithName(typefaceRef, size, NULL); // Use a default font
        
        NSDictionary* attributes = @{ 
            (id)kCTFontAttributeName: (__bridge id)font,
            (id)kCTForegroundColorAttributeName: (__bridge id)CGColorCreateGenericRGB(color.red, color.green, color.blue, color.alpha)
        };

        NSAttributedString* attributedString = [[NSAttributedString alloc] initWithString:nsText attributes:attributes];
        
        CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)attributedString);

        CGFloat maxTextWidth = CGFLOAT_MAX;

        if (maxWidth > 0) {
            maxTextWidth = (CGFloat)maxWidth;
        }

        CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, CFRangeMake(0, nsText.length), NULL, CGSizeMake(maxTextWidth, CGFLOAT_MAX), NULL);
        
        size_t width = ceil(suggestedSize.width);
        size_t height = ceil(suggestedSize.height);

        
        CGContextRef context = CGBitmapContextCreate(NULL, width, height, 8, width * 4, CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB), kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        
        if (!context) {
            CFRelease(framesetter);
            CFRelease(font);
            GLog(WARNING, "Failed to make bitmap context!");

            return (GRawImage){NULL, 0, 0};
        }
        
        CGContextSetTextMatrix(context, CGAffineTransformIdentity);

        CGMutablePathRef path = CGPathCreateMutable();
        CGPathAddRect(path, NULL, CGRectMake(0, 0, width, height));

        CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, nsText.length), path, NULL);
        CTFrameDraw(frame, context);

        uint32_t* pixelData = (uint32_t*)CGBitmapContextGetData(context);

        GRawImage rawImage;
        rawImage.width = width;
        rawImage.height = height;
        rawImage.rawData = (uint32_t*)malloc(sizeof(uint32_t) * width * height);


        if (!rawImage.rawData) {
            CGContextRelease(context);
            CGPathRelease(path);
            CFRelease(frame);
            CFRelease(framesetter);
            CFRelease(font);
            GLog(WARNING, "Failed to allocate image data!");
            return (GRawImage){NULL, 0, 0};
        }

        memcpy(rawImage.rawData, pixelData, sizeof(uint32_t) * width * height);

        CGContextRelease(context);
        CGPathRelease(path);
        CFRelease(frame);
        CFRelease(framesetter);
        CFRelease(font);

        GLog(INFO, "Loaded text \'%s\' in %luus", text, clock() - start);
        return rawImage;
    }
}