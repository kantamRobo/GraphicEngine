#pragma once
class CoreGraphicsEngine
{
public:



	bool InitializeEngine(GLFWwindow* window, unsigned int frameBufferWidth, unsigned int frameBufferHeight);

	bool CreateDevice();

	bool CreateCommandPool();
	bool CreateCommandBuffer();
	bool CreateSwapChain();
	VkDevice  m_Device = nullptr;
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

	VkRenderPass m_renderpass = nullptr;
	 
   
	VkDescriptorSet fbheap = nullptr;
	VkDescriptorSet dsvheap = nullptr;
	VkSemaphore     m_semaphore = nullptr;
	VkFence         m_fence = nullptr;
};

