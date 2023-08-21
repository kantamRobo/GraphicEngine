#pragma once
#include "UniformBuffer.h"

class DescriptorSet
{
	//コンストラクタ
	DescriptorSet();

	//デストラクタ
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
		MAX_SHADER_RESOURCE = 1024 * 10,	//シェーダーリソースの最大数。
		MAX_UNIFORM_BUFFER = 1024 * 10,	//定数バッファの最大数。
		MAX_SAMPLER_STATE = 16,	//サンプラステートの最大数。
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

