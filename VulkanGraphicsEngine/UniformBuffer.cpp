#include "UniformBuffer.h"
#include "stdafx.h"

void UniformBuffer::InitUniformBuffer(int size, void* srcData, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags)
{
	m_size = size;

	auto device = g_graphicEngine->GetDevice();

	//todo:Vulkan�ł�256�o�C�g�A���C�������g���K�v�����ׂ�

	int bufferNo = 0;
	VkBufferCreateInfo createbufferinfo = {};
	//�������񃉃{�̃T���v������q�؂����̂ŁA���p��
	//�����Ȃ��\��������B�����h�L�������g��I�[�v���\�[�X�̃G���W���̃\�[�X�R�[�h��ǂ�
	//���H�I�ȏ󋵂ɑΉ��ł���悤�ɂ��Ă�������
	createbufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createbufferinfo.usage = usage;
	createbufferinfo.size = size;

	auto result = vkCreateBuffer(device, &createbufferinfo, nullptr, &m_uniformBuffer[bufferNo]);

	VkMemoryRequirements reqs;  

	vkGetBufferMemoryRequirements(device, m_uniformBuffer[bufferNo], &reqs);

	VkMemoryAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	info.allocationSize = reqs.size;
	//�������^�C�v�̔���
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
