#pragma once

class DescriptorSet
{
	//コンストラクタ
	DescriptorSet();

	//デストラクタ
	~DescriptorSet();

	
private:

	VkDescriptorSet m_descriptorSet[2] = {};
	
	std::vector<IShaderResource*> m_ShaderResources;

};

