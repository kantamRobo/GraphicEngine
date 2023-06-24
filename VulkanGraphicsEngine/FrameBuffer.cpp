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

bool FrameBuffer::CreateDescriptorPool(VulkanGraphicsEngine& ge, VkDevice device)
{

	const uint32_t count = 100;
	std::array<VkDescriptorPoolSize, 2> descPoolSize;
	descPoolSize[0].descriptorCount = count;
	descPoolSize[0].type = VK_DESCRIPTOR_TYPE_IMAGE_
	descPoolSize[1].descriptorCount = count;
	descPoolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	//uint32_t maxDescriptorCount = uint32_t(m_swapchainImages.size() * m_model.meshes.size());
	unsigned int FrameBuffermaxDescriptorCount = 2;
	VkDescriptorPoolCreateInfo frameci{};
	frameci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	frameci.maxSets = FrameBuffermaxDescriptorCount;
	frameci.poolSizeCount = uint32_t(descPoolSize.size());
	frameci.pPoolSizes = descPoolSize.data();
	vkCreateDescriptorPool(m_device, &frameci, nullptr, &m_FrameBuffertextureDP);
	return false;
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
