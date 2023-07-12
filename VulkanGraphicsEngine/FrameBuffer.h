#pragma once
#include "stdafx.h"
#include <array>
#include "CoreGraphicsEngine.h"
class FrameBuffer
{
public:
	

	bool InitFrameBuffer(CoreGraphicsEngine* in_engine);

	void CreateTextureforFramebuffer(const VkRenderPass& renderpass, unsigned int width, unsigned int height);

private:
	CoreGraphicsEngine* m_coreGraphicsengine = nullptr;
	std::array<VkImageView,2> m_framebuffers;
	std::array<VkSwapchainCreateInfoKHR, 2> m_swapchainviews;
};
