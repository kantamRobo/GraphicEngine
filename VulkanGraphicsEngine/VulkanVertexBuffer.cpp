#include "VulkanVertexBuffer.h"
#include "stdafx.h"

void VulkanVertexBuffer::InitVKVertexBuffer(int size, int stride)
{
	auto VkDevice = vkGraphicEngine->GetDevice();
	VkBufferCreateInfo info = {};

	info.sType =
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		info.size = size;
	auto result = vkCreateBuffer(VkDevice, &info, nullptr, &m_vertexbuffer);

	//�������ʂ̎Z�o
	VkMemoryRequirements reqs;

	vkGetBufferMemoryRequirements(VkDevice, m_vertexbuffer,&reqs);

	VkMemoryAllocateInfo alinfo{};

	alinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alinfo.allocationSize = reqs.size;

	//�������^�C�v�̔���
	auto flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	alinfo.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
	
	vkAllocateMemory(VkDevice, &alinfo, nullptr, memory);

	//�������̃o�C���h

	vkBindBufferMemory(VkDevice, m_vertexbuffer, memory, 0);

	

}

void VulkanVertexBuffer::Copy(void* srcVertices)
{
	uint8_t* pData;
	vkMapMemory(device,memory, 0, VK_WHOLE_SIZE, 0, &pdata);
	memcpy(p, srcVertices, sizeof(srcVertices));
	vkUnmapMemory(device, memory);

}
