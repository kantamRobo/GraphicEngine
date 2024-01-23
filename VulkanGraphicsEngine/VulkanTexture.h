#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "CoreGraphicsEngine.h"
class VulkanTexture:public IVulkanShaderResource
{
public:
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

	int width;
	int height;
	VkImage m_texture = nullptr;
	VkImageCreateInfo m_textureInfo;
	//�X�e�[�W���O�o�b�t�@������H
	
	
	void LoadTextureFromStorage(CoreGraphicsEngine& coregraphicsengine, VkDevice device, const std::string& filepath, VkFormat& format, void* initialData);

};

