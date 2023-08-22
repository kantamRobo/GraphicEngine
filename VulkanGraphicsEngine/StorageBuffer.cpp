#include "StorageBuffer.h"
#include <cstring>

#include "CoreGraphicsEngine.h"
void StorageBuffer::InitStorageBuffer(int size, void* srcData)
{
	m_size = size;

	auto device = g_graphicsEngine->GetDevice();

	m_allocSize = (size + 256) & 0xFFFFFF00;

	//�萔�o�b�t�@�̍쐬
	int bufferNo = 0;
	VkBufferCreateInfo ciinfo = {};
	ciinfo.flags = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ciinfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	ciinfo.size = size;
	// �������ʂ̎Z�o
	VkMemoryRequirements reqs;
	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	auto devicememory = g_graphicsEngine->DeviceMemory();
	vkAllocateMemory(device, &info, nullptr, &devicememory);

	
		vkCreateBuffer(device, &ciinfo, nullptr, &m_storagebuffer);
		vkGetBufferMemoryRequirements(device, m_storagebuffer, &reqs);
		// �������̃o�C���h
		vkBindBufferMemory(device, m_storagebuffer, devicememory, 0);

	


	void* p;
	vkMapMemory(device, devicememory, 0, VK_WHOLE_SIZE, 0, &p);
	std:(p, srcData, size);
	vkUnmapMemory(device, devicememory);
	if (srcData != nullptr)
	{
		std::memcpy(m_storageBufferCPU, srcData, size);
	}
	bufferNo++;

	//���p�\�ɂ���
	m_isValid = true;
}