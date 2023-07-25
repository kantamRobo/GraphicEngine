#include <GLFW/glfw3.h>
#include <array>
#include "stdafx.h"
#include "CoreGraphicsEngine.h"


#include <vector>
#include "RenderingContext.h"


CoreGraphicsEngine* g_graphicsEngine = nullptr;	//グラフィックスエンジン
Camera* g_camera2D = nullptr;				//2Dカメラ。
Camera* g_camera3D = nullptr;				//3Dカメラ。
bool CoreGraphicsEngine::InitializeEngine(GLFWwindow* window, unsigned int frameBufferWidth, unsigned int frameBufferHeight)
{
	g_graphicsEngine = this;

	//デバイス作成
	//論理デバイス
	
	
	//物理デバイス

	return false;
}

bool CoreGraphicsEngine::CreateDevice()
{
	const float defaultQueuePriority(1.0f);
	VkDeviceQueueCreateInfo devQueueCI{};
	devQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	devQueueCI.queueFamilyIndex = m_graphicsQueueIndex;
	devQueueCI.queueCount = 1;
	devQueueCI.pQueuePriorities = &defaultQueuePriority;


	std::vector<VkExtensionProperties> devExtProps;
	{
		uint32_t count = 0;
		vkEnumerateDeviceExtensionProperties(PDevice, nullptr, &count, nullptr);

		devExtProps.resize(count);
		vkEnumerateDeviceExtensionProperties(PDevice, nullptr, &count, devExtProps.data());

		std::vector<const char*> extensions;

		for (const auto& v : devExtProps)
		{
			extensions.push_back(v.extensionName);
		}

		VkDeviceCreateInfo ci{};

		ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		ci.pQueueCreateInfos = &devQueueCI;
		ci.queueCreateInfoCount = 1;
		ci.enabledExtensionCount = uint32_t(extensions.size());

		auto result = vkCreateDevice(PDevice, &ci, nullptr, &m_Device);
		if (!result)
		{
			return false;
		 }

		//デバイスキューの取得
		vkGetDeviceQueue(m_Device, m_graphicsQueueIndex, 0, &devicequeue);
	}
	return true;
}

void CoreGraphicsEngine::CreateSurfaceFormat(VkFormat format)
{
	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(PDevice, surface, &surfaceFormatCount, nullptr);
	std::vector<VkSurfaceFormatKHR>formats(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(PDevice, surface, &surfaceFormatCount,formats.data() );
	for (const auto& f : formats)
	{
		if (f.format == format) {
			surfaceFormat = f;
		}
	}

	
}

bool CoreGraphicsEngine::CreateRenderPass()
{
	VkRenderPassCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	std::array<VkAttachmentDescription, 2> attachments;
	auto& colorTarget = attachments[0];
	auto& depthTarget = attachments[1];

	colorTarget = VkAttachmentDescription{};
	colorTarget.format = surfaceFormat.format;
	colorTarget.samples = VK_SAMPLE_COUNT_1_BIT;



	return false;
}

bool CoreGraphicsEngine::CreateCommandPool()
{
	VkCommandPoolCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	ci.queueFamilyIndex = m_graphicsQueueIndex;
	ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VkResult result = vkCreateCommandPool(m_Device, &ci, nullptr, &commandpool);
	if (!result != VK_SUCCESS)
	{
		return false;
	}
	return true;
}

bool CoreGraphicsEngine::CreateSwapChain(GLFWwindow* window)
{
	auto imageCount = (std::max)(2u, surfaceCaps.minImageCount);
	auto extent = surfaceCaps.currentExtent;
	if (extent.width == ~0u)
	{
		// 値が無効なのでウィンドウサイズを使用する.
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		extent.width = uint32_t(width);
		extent.height = uint32_t(height);
	}

	uint32_t queueFamilyIndices[] = { m_graphicsQueueIndex };
	VkSwapchainCreateInfoKHR ci{};
	ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	ci.surface = surface;
	ci.minImageCount = imageCount;
	ci.imageFormat = surfaceFormat.format;
	ci.imageColorSpace = surfaceFormat.colorSpace;
	ci.imageExtent = extent;
	ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	ci.preTransform = surfaceCaps.currentTransform;
	ci.imageArrayLayers = 1;
	ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	ci.queueFamilyIndexCount = 0;
	ci.presentMode = presentMode;
	ci.oldSwapchain = VK_NULL_HANDLE;
	ci.clipped = VK_TRUE;
	ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	auto result = vkCreateSwapchainKHR(m_Device, &ci, nullptr, &swapchain);
	if (result != VK_SUCCESS)
	{
		return false;
}
	swapchainExtent = extent;
	return false;
}

bool CoreGraphicsEngine::CreateCommandBuffer()
{
	VkCommandBufferAllocateInfo ai{};
	ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	ai.commandPool = commandpool;
	ai.commandBufferCount = uint32_t(swapchainViews.size());
	ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandbuffer.resize(ai.commandBufferCount);
	auto result = vkAllocateCommandBuffers(m_Device,&ai, commandbuffer.data());

	if (result != VK_SUCCESS)
	{

		return false;
	}
	return true;
}

bool CoreGraphicsEngine::CreateFence()
{
	return false;
}

