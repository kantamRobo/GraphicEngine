#include "VulkanGPUBuffer.h"

void VulkanGPUBuffer::Init(VkDevice device,const VkBufferCreateInfo& info)
{
	//info������

	//�o�b�t�@�𐶐�����
	vkCreateBuffer(device, &info, nullptr, &m_buffer);

}
