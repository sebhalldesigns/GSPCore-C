#ifndef GFRAME_H
#define GFRAME_H

// The GFrame type describes a plaform-independant in-memory bitmap

typedef void* GFrame;

typedef struct {
    float red;
    float green;
    float blue;
    float alpha;
} GColor;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} GRect;


typedef struct {
    int width;
    int height;
} GFrameInfo;



GFrame GFrame_Alloc(GFrameInfo info);
void GFrame_Free(GFrame frame);

void GFrame_Fill(GFrame frame, GRect rect, GColor color);


#endif // GFRAME_H