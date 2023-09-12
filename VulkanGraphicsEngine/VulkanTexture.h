#pragma once
#include <vulkan/vulkan.h>
class VulkanTexture:public IVulkanShaderResource
{

	//�R���X�g���N�^
	VulkanTexture(){}
	//�f�X�g���N�^
	~VulkanTexture();

	///
	///�t�@�C������e�N�X�`�������[�h����R���X�g���N�^
	/// 
	/// ���[�h����e�N�X�`���̃t�@�C���p�X�B
	
	explicit VulkanTexture(const wchar_t* filePath);

	VkImage Get()
	{
		return m_texture;
	}
	//�e�N�X�`���̕����擾
	int GetWidth()const
	{
		m_texture.
	}
	//�e�N�X�`���̍������擾
	int GetHeight()const
	{
		
	}

	///�e�N�X�`���̃t�H�[�}�b�g���擾
	VkFormat GetFormat()const
	{
		return m_textureInfo.format;
	}

private:

	VkImage m_texture = nullptr;
	VkImageViewCreateInfo m_textureInfo;
};

