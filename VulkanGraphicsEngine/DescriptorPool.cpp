#include "stdafx.h"

#include "IVulkanShaderResource.h"
#include "IVulkanUnorderedResource.h"

#include "DescriptorPool.h"



void DescriptorPool::Commit()
{
	const auto& Device = g_graphicEngine->GetDevice();

	
	VkDescriptorSetLayoutBinding bindingUBO{};
	bindingUBO.binding = 0;
	bindingUBO.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	bindingUBO.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	bindingUBO.descriptorCount = 1;
	

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = 1; // バインディングの数
	descriptorSetLayoutCreateInfo.pBindings = &bindingUBO; // バインディングの配列
	
	
	for (int i = 0; i < sizeof(m_uniformbuffer); i++) {
		std::array<VkDescriptorPoolSize, 2> descpoolSize{};
		descpoolSize[0].descriptorCount = uint32_t(m_uniformbuffer[i]->GetBuffer().size());
		descpoolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descpoolSize[1].descriptorCount = uint32_t(m_uniformbuffer[i]->GetBuffer().size());
		descpoolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		VkDescriptorPoolCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		ci.maxSets = uint32_t(m_uniformbuffer[i]->GetBuffer().size());
		ci.poolSizeCount = uint32_t(descpoolSize.size());
		ci.pPoolSizes = descpoolSize.data();
		for (auto& dp : m_descriptorPool) {
			vkCreateDescriptorPool(m_device, &ci, nullptr, &dp);
		}

	}

}


