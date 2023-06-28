#include "UniformBuffer.h"
#include "stdafx.h"

void UniformBuffer::InitUniformBuffer(int size, void* srcData, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags)
{
	m_size = size;

	auto device = g_graphicEngine->GetDevice();

	//todo:Vulkanでも256バイトアラインメントが必要か調べる

	int bufferNo = 0;
	VkBufferCreateInfo createbufferinfo = {};
	//旧すらりんラボのサンプルから拝借したので、実用に
	//堪えない可能性がある。公式ドキュメントやオープンソースのエンジンのソースコードを読み
	//実践的な状況に対応できるようにしておくこと
	createbufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createbufferinfo.usage = usage;
	createbufferinfo.size = size;

	auto result = vkCreateBuffer(device, &createbufferinfo, nullptr, &m_uniformBuffer[bufferNo]);

	VkMemoryRequirements reqs;  

	vkGetBufferMemoryRequirements(device, m_uniformBuffer[bufferNo], &reqs);

	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	//メモリタイプの判定
	info.memoryTypeIndex = getMemoryTypeIndex(reqs.memoryTypeBits, flags);
}


uint32_t UniformBuffer::getMemoryTypeIndex(uint32_t requestBits, VkMemoryPropertyFlags requestProps)const
{
	uint32_t result = ~0u;
	for (uint32_t i = 0; i < m_physMemProps.memoryTypeCount; ++i)
	{
		if (requestBits & 1)
		{
			const auto& types = m_physMemProps.memoryTypes[i];
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

void UniformBuffer::CopyToVRAM(void* data)
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	memcpy(m_uniformBufferCPU[backBufferIndex], data, m_size);
}
