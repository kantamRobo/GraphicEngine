#pragma once
class VulkanPipelineState
{
public:
	~VulkanPipelineState()
	{
		if (m_pipeline)
		{
			m_pipeline = nullptr;
		}
	}
	void InitPipeline();
	
private:
	VkPipeline m_pipeline = nullptr;
	std::unique_ptr<VulkanGraphicsEngine> m_graphicsEngine = nullptr;
	
};

