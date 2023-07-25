#pragma once
class CoreGraphicsEngine
{
public:



	bool InitializeEngine(GLFWwindow* window, unsigned int frameBufferWidth, unsigned int frameBufferHeight);

	bool CreateDevice();
	void CreateSurfaceFormat(VkFormat format);
	bool CreateRenderPass();
	bool CreateCommandPool();
	bool CreateSwapChain(GLFWwindow* window);
	bool CreateCommandBuffer();
	bool CreateSemaphore();
	VkDevice  m_Device = nullptr;
	VkPhysicalDevice PDevice = nullptr;
	VkDeviceMemory DeviceMemory = nullptr;
	
	//Command
	VkCommandPool   commandpool = nullptr;
	std::vector<VkCommandBuffer> commandbuffer = {};
	VkQueue devicequeue = nullptr;

	//Surface
	//SwapChain
	VkExtent2D    swapchainExtent;
	VkSurfaceCapabilitiesKHR  surfaceCaps;
	VkSurfaceKHR surface;
	VkSurfaceFormatKHR surfaceFormat;
	VkSurfaceFormatKHR presentMode;
	VkSwapchainKHR swapchain;
	VkPresentModeKHR presentMode;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainViews;
	VkBuffer depthstencilbuffer = nullptr;

	uint32_t m_graphicsQueueIndex;
	VkViewport viewport;
	VkRect2D   rect;

	RenderingContext renderingcontext;
	//Renderpass

	VkRenderPass m_renderpass = nullptr;
	
	std::vector<VkFence>  m_fences = {};
	VkDescriptorSet fbheap = nullptr;
	VkDescriptorSet dsvheap = nullptr;
	VkSemaphore   m_renderCompletedSem, m_presentCompletedSem;

	
};

