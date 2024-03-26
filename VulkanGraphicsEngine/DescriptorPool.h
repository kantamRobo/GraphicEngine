#pragma once
#include "UniformBuffer.h"
#include <vector>
class DescriptorPool
{
public:
	//コンストラクタ
	DescriptorPool();

	//デストラクタ
	~DescriptorPool();

	VkDescriptorPool Get()const;

	void RegistShaderResource(int registerNo, IVulkanShaderResource& sr)
	{
		
	}

	void RegistUnorderAccessResource(int registerNo, IVulkanUnorderdResrouce& unr)
	{

	}



	void Commit();
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
	VkDescriptorPool m_descriptorPool[2] = {};

	/*
	std::vector<IVulkanShaderResource*> m_shaderResource = {};
	std::vector<IVulkanUnorderdResrouce*> m_uavResource;
	std::vector<UniformBuffer*> m_uniformbuffer = {};
	*/
	このポインタ動的配列は危険なので、別の方法を考える。
	
	
	VkSamplerCreateInfo info;
	
	

};

