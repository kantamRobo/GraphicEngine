#pragma once
#include <vulkan/vulkan.h>
class VulkanTLASBuffer
{

	//�C���X�^���X
	//�o�b�t�@����(�o�b�t�@�E�X�N���b�`�o�b�t�@)
	VkBuffer m_tlasbuffer;
	VkBuffer m_tlasScracthbuffer;
	//�f�o�C�X����������
	VkDeviceMemory tlasbuffermemory;
	VkDeviceMemory tlasscratchbuffermemory;
	void InitTLAS();

};

