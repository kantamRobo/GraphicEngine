#pragma once
#include "RenderingContext.h"
#include "stdafx.h"


void RenderingContext::SetFrameBufferAndViewport(FrameBuffer& framebuffer)
{
	VkViewport viewport = {};
	
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = static_cast<float>(framebuffer.GetWidth());
	viewport.height = static_cast<float>(framebuffer.GetHeight());
	viewport.minDepth = MIN_DEPTH;
	viewport.maxDepth = MAX_DEPTH;


	SetViewportAndScissor(viewport);
	
}
