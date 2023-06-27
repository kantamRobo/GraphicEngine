#include "VulkanPipelineState.h"
#include "VulkanGraphicsEngine.h"
#include "stdafx.h"
void VulkanPipelineState::InitGraphicPipeline(const VkGraphicsPipelineCreateInfo& info)
{
	
	auto Device = m_graphicsengine->GetDevice();

	auto hr = vkCreateGraphicsPipelines(Device, VK_NULL_HANDLE, 1, &info, nullptr, &m_graphicpipeline);
	
}

void VulkanPipelineState::InitComputePipeline(const VkComputePipelineCreateInfo& info)
{
	auto Device = m_graphicsengine->GetDevice();

	auto hr = vkCreateComputePipelines(Device, VK_NULL_HANDLE, 1, &info, nullptr, &m_computepipeline);
}