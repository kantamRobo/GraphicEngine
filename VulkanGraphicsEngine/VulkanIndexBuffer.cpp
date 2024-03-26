#include "VulkanIndexBuffer.h"
#include "stdafx.h"
#include <memory>

void VulkanIndexBuffer::InitVulkanIndexBuffer(int size, int stride)
{
	auto VkDevice = vkGraphicEngine->GetDevice();
	VkBufferCreateInfo info = {};

	info.sType =
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		info.size = size;
	auto result = vkCreateBuffer(VkDevice, &info, nullptr, &m_indexbuffer);

	//メモリ量の算出
	VkMemoryRequirements reqs;

	vkGetBufferMemoryRequirements(VkDevice, m_indexbuffer, &reqs);

	VkMemoryAllocateInfo alinfo{};

	alinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alinfo.allocationSize = reqs.size;

	//メモリタイプの判定
	auto flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	alinfo.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);

	vkAllocateMemory(VkDevice, &alinfo, nullptr, memory);

	//メモリのバインド

	vkBindBufferMemory(VkDevice, m_indexbuffer, memory, 0);



}

void VulkanIndexBuffer::Copy(void* srcIndices)
{

	void* pData;
	vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &pData);
	memcpy(pData, srcIndices, sizeof(srcIndices));
	vkUnmapMemory(device, memory);
}
