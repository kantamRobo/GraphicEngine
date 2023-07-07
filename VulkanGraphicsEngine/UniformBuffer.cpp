#include "UniformBuffer.h"
#include "stdafx.h"

void UniformBuffer::InitUniformBuffer(int size, void* srcData)
{
	m_size = size;

	auto device = g_graphicsEngine->GetDevice();

	m_allocSize = (size + 256) & 0xFFFFFF00;

	//�萔�o�b�t�@�̍쐬
	int bufferNo = 0;
	VkBufferCreateInfo ciinfo = {};
	ciinfo.flags = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ciinfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	ciinfo.size = size;
	// �������ʂ̎Z�o
	VkMemoryRequirements reqs;
	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	auto devicememory = g_graphicsEngine->DeviceMemory();
	vkAllocateMemory(device, &info, nullptr, &devicememory);

	for (auto& ub : m_uniformbuffer) {
		vkCreateBuffer(device, &ciinfo, nullptr, &ub);
		vkGetBufferMemoryRequirements(device, &ub, &reqs);
		// �������̃o�C���h
		vkBindBufferMemory(device, ub, devicememory, 0);

	}
	

	void* p;
	vkMapMemory(device, devicememory, 0, VK_WHOLE_SIZE, 0, &p);
	memcpy(p, srcData, size);
	vkUnmapMemory(device, devicememory);
	if (srcData != nullptr)
	{
		memcpy(m_uniformBufferCPU[bufferNo], srcData, size);
	}
	bufferNo++;
	
	//���p�\�ɂ���
	m_isValid = true;
}

void UniformBuffer::CopyToVRAM(void* data)
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	memcpy(m_uniformBufferCPU[backBufferIndex], data, m_size);
}

