#pragma once
#include "stdafx.h"
#include <array>
#include "CoreGraphicsEngine.h"
class FrameBuffer
{
public:
	

	
	
	bool InitFrameBuffer(CoreGraphicsEngine* in_engine, const unsigned int width, const unsigned int height, VkFormat depthstencilformat);

	void CreateTextureforFramebuffer(const VkRenderPass& renderpass, unsigned int width, unsigned int height);

	bool CreateDepthStencilTexture(CoreGraphicsEngine* ge, const unsigned int width, const unsigned int height, VkFormat format);

	
	
	
private:
	CoreGraphicsEngine* m_coreGraphicsengine = nullptr;
	VkImage framebuffertexture;
	VkImage   depthstencil;
	VkDeviceMemory  framebuffermemory;
	VkImageView depthstencilview;
	VkBuffer depthstencilbuffer;
	std::array<VkImageView,2> m_framebuffers;
	std::array<VkSwapchainCreateInfoKHR, 2> m_swapchainviews;
};
