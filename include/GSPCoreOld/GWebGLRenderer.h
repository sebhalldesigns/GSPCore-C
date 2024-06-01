#ifndef GWEBGLRENDERER_H
#define GWEBGLRENDERER_H

#include "GSPCore/UI/GView.h"

// static class, no instances

// try to init, return true if successful and false if unsuccessful
bool GWebGLRenderer_Init();

void GWebGLRenderer_RenderView(GView view);
void GWebGLRenderer_SetViewportSize(double width, double height);

void GWebGLRenderer_InitForView(GView view);
void GWebGLRenderer_FreeForView(GView view);

GSize GWebGLRenderer_MeasureTextForView(GView view, char* text);
void GWebGLRenderer_RenderTextForView(GView view, char* text);



#endif // GWEBGLRENDERER_H