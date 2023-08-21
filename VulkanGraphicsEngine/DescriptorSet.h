#pragma once
#include "UniformBuffer.h"

class DescriptorSet
{
	//�R���X�g���N�^
	DescriptorSet();

	//�f�X�g���N�^
	~DescriptorSet();

	VkDescriptorSet Get()const;

	void RegistShaderResource(int registerNo, IVulkanShaderResource& sr)
	{
		
	}

	void RegistUnorderAccessResource(int registerNo, IVulkanUnorderdResrouce& unr)
	{

	}
private:
	enum {
		MAX_SHADER_RESOURCE = 1024 * 10,	//�V�F�[�_�[���\�[�X�̍ő吔�B
		MAX_UNIFORM_BUFFER = 1024 * 10,	//�萔�o�b�t�@�̍ő吔�B
		MAX_SAMPLER_STATE = 16,	//�T���v���X�e�[�g�̍ő吔�B
	};

	
	std::vector<IVulkanShaderResource*> m_ShaderResources;

	int m_numShaderResource = 0;
	int numUniformBuffer = 0;
	int m_numUavResource = 0;
	VkDescriptorSet m_descriptorSet[2] = {};

	std::vector<IVulkanShaderResource*> m_shaderResource = {};
	std::vector<IVulkanUnorderdResrouce*> m_uavResource;
	std::vector<UniformBuffer*> m_uniformbuffer = {};
	VkSamplerCreateInfo info;
};

