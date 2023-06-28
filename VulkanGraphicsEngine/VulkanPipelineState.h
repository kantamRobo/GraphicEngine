#pragma once

class VulkanPipelineState
{
public:
	VulkanPipelineState() {};

	VulkanPipelineState(){ m_graphicsengine = std::make_shared<VulkanGraphicsEngine>(); }

	~VulkanPipelineState()
	{
		if (m_graphicpipeline)
		{
			m_graphicpipeline = nullptr;
		}
	}
	
	void InitGraphicPipeline(const VkGraphicsPipelineCreateInfo& info);
	void InitComputePipeline(const VkComputePipelineCreateInfo& info);
private:

	VkPipeline m_graphicpipeline = VK_NULL_HANDLE;
	VkPipeline m_computepipeline = VK_NULL_HANDLE;
	std::shared_ptr<VulkanGraphicsEngine> m_graphicsEngine = nullptr;

	

};

