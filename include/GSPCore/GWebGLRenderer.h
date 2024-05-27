#ifndef GWEBGLRENDERER_H
#define GWEBGLRENDERER_H

#include "GSPCore/UI/GView.h"

// static class, no instances

// try to init, return true if successful and false if unsuccessful
bool GWebGLRenderer_Init();

void GWebGLRenderer_RenderView(GView view);





#endif // GWEBGLRENDERER_H