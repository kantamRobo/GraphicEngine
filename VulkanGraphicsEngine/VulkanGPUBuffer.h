#pragma once
#include <vulkan/vulkan.h>
class VulkanGPUBuffer
{
public:
	~VulkanGPUBuffer()
	{
		if (m_buffer != nullptr)
		{
			m_buffer = nullptr;
		}


	}
	
	void Init(VkDevice device, const VkBufferCreateInfo& info);

private:
	VkBuffer m_buffer = nullptr;
	VkBufferCreateInfo m_desc = {};
	};

