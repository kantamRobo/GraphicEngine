#pragma once
class VulkanIndexBuffer
{
public:
	//デストラクタ
	~VulkanIndexBuffer();

	void InitVulkanIndexBuffer(int size, int stride);

	//頂点データを頂点バッファにコピー
	void Copy(void* srcVertices);

	VkBuffer GetBufferAddress()const
	{
		return m_indexbuffer;
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
	VkBuffer m_indexbuffer;
	VkPhysicalDeviceMemoryProperties m_physMemProps;
	VkDeviceMemory memory;




};

//https://magazine.cygames.co.jp/archives/11329#i2

