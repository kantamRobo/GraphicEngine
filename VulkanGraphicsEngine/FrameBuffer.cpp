#include "stdafx.h"
#include "FrameBuffer.h"
#include "VulkanTexture.h"
#include "VulkanGraphicsEngine.h"
#include <array>
unsigned int FrameBuffer::getTextureMemoryTypeIndex(unsigned int requestBits, VkMemoryPropertyFlags requestProps, VkPhysicalDeviceMemoryProperties physMemProps)
{
	uint32_t result = ~0u;
	for (uint32_t i = 0; i < physMemProps.memoryTypeCount; ++i)
	{
		if (requestBits & 1)
		{
			const auto& types = physMemProps.memoryTypes[i];
			if ((types.propertyFlags & requestProps) == requestProps)
			{
				result = i;
				break;
			}
		}
		requestBits >>= 1;
	}
	return result;
}

bool FrameBuffer::CreateFrameBuffer(int w, int h, int mipLevel, VkFormat colorformat, VkFormat depthstencilFormat, float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps)
{
	auto Device = g_graphicengine->GetDevice();
	
	m_width = w;
	m_height = h;

	if (!CreateFramebufferTexture(*g_graphicengine, Device, m_width, m_height, mipLevel, clearColor, physMemProps));
	{
		return false;
	}

	if (depthstencilFormat != VK_FORMAT_UNDEFINED)
	{
		if (!CreateDepthStencilTexture(*g_graphicengine, Device, m_width, m_height, depthstencilFormat))
		{
			return false;
	  }
	}
	if (!CreateDescriptorPool(*g_graphicengine, Device))
	{

		return false;
	}
	return true;
}

bool FrameBuffer::CreateFramebufferTexture(VulkanGraphicsEngine& ge, VkDevice Device, int w, int h, int miplevel,float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps)
{

	VkClearValue clearvalue;
	if (clearColor != nullptr) {
		clearvalue.color.float32[0] = clearColor[0];
		clearvalue.color.float32[1] = clearColor[1];
		clearvalue.color.float32[2] = clearColor[2];
		clearvalue.color.float32[3] = clearColor[3];
	}
	else
	{
		clearvalue.color.float32[0] = 0;
		clearvalue.color.float32[1] = 0;
		clearvalue.color.float32[2] = 0;
		clearvalue.color.float32[3] = 0;
	}
	VkBufferCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	unsigned int imagesize = w * h * sizeof(unsigned int);
	ci.size = imagesize;

	auto result = vkCreateBuffer(Device, &ci, nullptr, &m_framebuffertextureVulkan);

	//ÉÅÉÇÉäó ÇÃéZèo
	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(Device, m_framebuffertextureVulkan, &reqs);
	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	info.memoryTypeIndex = getTextureMemoryTypeIndex(reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, physMemProps);
	//ÉÅÉÇÉäÇÃämï€
	vkAllocateMemory(Device, &info, nullptr, &FrameBufferdevicememory);


}

bool FrameBuffer::CreateImageView(VulkanGraphicsEngine& ge, VkDevice device)
{

	VkImageCreateInfo depthci{};
	depthci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthci.imageType = VK_IMAGE_TYPE_2D;
	depthci.format = VK_FORMAT_D32_SFLOAT;
	depthci.extent.width = m_width;
	depthci.extent.height = m_height;
	depthci.extent.depth = 1;
	depthci.mipLevels = 1;
	depthci.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthci.samples = VK_SAMPLE_COUNT_1_BIT;
	depthci.arrayLayers = 1;
	auto depthesult = vkCreateImage(m_device, &depthci, nullptr, &m_depthImage);
	if (!depthesult)
	{
		return false;
	}
	VkImageCreateInfo framebufferci{};
	framebufferci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	framebufferci.imageType = VK_IMAGE_TYPE_2D;
	framebufferci.format = VK_FORMAT_D32_SFLOAT;
	framebufferci.extent.width = m_width;
	framebufferci.extent.height = m_height;
	framebufferci.extent.depth = 1;
	framebufferci.mipLevels = 1;
	framebufferci.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	framebufferci.samples = VK_SAMPLE_COUNT_1_BIT;
	framebufferci.arrayLayers = 1;
	auto frameresult = vkCreateImage(m_device, &framebufferci, nullptr, &m_FrameBufferImage);
	if (!frameresult) {
		return false;
	}

	return true;
}



bool FrameBuffer::CreateDepthStencilTexture(VulkanGraphicsEngine& ge,
	const VkDevice& Device,
	int w,
	int h,
	VkFormat format)
{

	VkBufferCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	unsigned int imagesize = w * h * sizeof(unsigned int);
	ci.size = imagesize;

	auto result = vkCreateBuffer(Device, &ci, nullptr, &m_depthStencilTexture);
	if (!result)
	{
		return false;
	}

	return true;
}
