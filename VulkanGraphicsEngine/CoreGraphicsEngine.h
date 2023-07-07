#pragma once
class CoreGraphicsEngine
{

	VkDevice  Device = nullptr;
	VkPhysicalDevice PDevice = nullptr;
	VkDeviceMemory DeviceMemory = nullptr;
	
	//Command
	VkCommandPool   commandpool = nullptr;
	VkCommandBuffer commandbuffer = nullptr;
	VkQueue devicequeue = nullptr;

	//Surface
	//SwapChain
	VkSurfaceKHR surface;
	VkSurfaceFormatKHR presentMode;
	VkSwapchainKHR swapchain;
	VkPresentModeKHR presentMode;
	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainViews;
	VkBuffer depthstencilbuffer = nullptr;

	uint32_t m_graphicsQueueIndex;
	VkViewport viewport;
	VkRect2D   rect;

	RenderingContext renderingcontext;
	//Renderpass

	VkRenderPass renderpass = nullptr;
	 
   
	VkDescriptorSet fbheap = nullptr;
	VkDescriptorSet dsvheap = nullptr;

};

