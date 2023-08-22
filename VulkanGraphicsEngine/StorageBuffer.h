#pragma once
#include <vulkan/vulkan.h>
class StorageBuffer
{
public:
	void InitStorageBuffer(int size, void* srcData);
private:
	VkBuffer m_storagebuffer = { nullptr };
	void* m_storageBufferCPU= { nullptr };
	int m_size = 0;
	int m_allocSize = 0;
	bool m_isValid = false;
	
};

