#include <GLFW/glfw3.h>
#include <array>
#include "stdafx.h"
#include <vector>
#include "CoreGraphicsEngine.h"


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
	colorTarget.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorTarget.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorTarget.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorTarget.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorTarget.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorTarget.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	depthTarget = VkAttachmentDescription{};
	depthTarget.format = VK_FORMAT_D32_SFLOAT;
	depthTarget.samples = VK_SAMPLE_COUNT_1_BIT;
	depthTarget.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthTarget.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthTarget.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthTarget.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthTarget.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthTarget.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorReference{}, depthReference{};
  colorReference.attachment = 0;
  colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  depthReference.attachment = 1;
  depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpassDesc{};
  subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpassDesc.colorAttachmentCount = 1;
  subpassDesc.pColorAttachments = &colorReference;
  subpassDesc.pDepthStencilAttachment = &depthReference;

  ci.attachmentCount = uint32_t(attachments.size());
  ci.pAttachments = attachments.data();
  ci.subpassCount = 1;
  ci.pSubpasses = &subpassDesc;

  auto result = vkCreateRenderPass(m_Device, &ci, nullptr, &m_renderpass);
	
  if (result != VK_SUCCESS)
  {
	  return false;
  }
  return true;
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
	return true;
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

	//フェンスを作成
	 // コマンドバッファのフェンスも同数用意する.
	m_fences.resize(ai.commandBufferCount);
	VkFenceCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	ci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (auto& v : m_fences)
	{
		result = vkCreateFence(m_Device, &ci, nullptr, &v);
		checkResult(result);
	}

	return true;
}

bool CoreGraphicsEngine::CreateSemaphore()
{
	
	//セマフォを作成
	VkSemaphoreCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(m_Device, &ci, nullptr, &m_renderCompletedSem);
	vkCreateSemaphore(m_Device, &ci, nullptr, &m_presentCompletedSem);

	return true;
}

bool CoreGraphicsEngine::CreateDescriptorSetForFrameBuffer()
{
	auto imageCount = swapchainViews.size();
	VkDescriptorSetAllocateInfo descriptorSetInfo = {};
		descriptorSetInfo.descriptorPool = descpool;
		descriptorSetInfo.descriptorSetCount = 1;
		descriptorSetInfo.pNext = nullptr;
		descriptorSetInfo.pSetLayouts = &desclayout;
		descriptorSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

		for (uint32_t i = 0; i < imageCount; ++i)
		{
			
			auto resultFBset = vkAllocateDescriptorSets(m_Device, &descriptorSetInfo, &fbheap);
			//ThrowIfFailed(result, "vkAllocateDescriptorSets Failed.");
			//m_descriptorSets.push_back(descriptorSet);
		}
}

