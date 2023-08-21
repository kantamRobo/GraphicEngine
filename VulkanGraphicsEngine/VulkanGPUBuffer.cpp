#include "VulkanGPUBuffer.h"

void VulkanGPUBuffer::Init(VkDevice device,const VkBufferCreateInfo& info)
{
	//infoを書く

	//バッファを生成する
	vkCreateBuffer(device, &info, nullptr, &m_buffer);

}
