#ifndef GRENDERPASS_H
#define GRENDERPASS_H

#include "GFrame.h"
#include "GShader.h"
#include "GVertexBuffer.h"
#include "GTexture.h"

typedef void* GRenderPass;

GRenderPass GRenderPass_Alloc();
void GRenderPass_Free(GRenderPass renderPass);

void GRenderPass_SetShader(GShader shader);
void GRenderPass_SetVertexBuffer(GVertexBuffer vertexBuffer);
void GRenderPass_SetTexture(GTexture texture);
void GRenderPass_SetTarget(GFrame frame);

void GRenderPass_Commit(GRenderPass renderPass);

#endif // GRENDERPASS_H