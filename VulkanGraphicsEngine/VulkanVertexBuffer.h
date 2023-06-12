#pragma once
class VulkanVertexBuffer
{
public:
	//デストラクタ
	~VulkanVertexBuffer();

	void InitVKVertexBuffer(int size, int stride);

	//頂点データを頂点バッファにコピー
	void Copy(void* srcVertices);

	VkBuffer GetBufferAddress()const
	{
		return m_vertexbuffer;
	}


	uint32_t getMemoryTypeIndex(uint32_t requestBits, VkMemoryPropertyFlags requestProps)const
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
private:
	VkBuffer m_vertexbuffer;
	VkPhysicalDeviceMemoryProperties m_physMemProps;
	VkDeviceMemory memory;
};

