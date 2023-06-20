#pragma once
class RenderingContext
{//2023.6.7目標
//４つ実装
	void InitRenderingContext(std::shared_ptr<VkCommandBuffer> commandbuffer)
	{
		m_commandBuffer = commandbuffer;
	}

	//頂点バッファを設定
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> in_vb)
	{
		//TODO
		vkCmdBindVertexBuffers(m_commandBuffer, );
	}

	//インデックスバッファを設定
	void SetIndexBuffer(std::shared_ptr<Indexbuffer> ib)
	{
		//TODO
		vkCmdBindIndexBuffer(m_commandBuffer);
	}

	void SetPrimitiveTopology(VkPrimitiveTopology topology)
	{
		vkCmdSetPrimitiveTopology(m_commandBuffer, topology);
		//
	}
    

	//ビューポートとシザリング矩形をセットで設定

	void SetViewportAndScissor(VkViewport& viewport)
	{
		VkRect2D rect = {};
		rect.extent.width = viewport.width;
		rect.extent.height = viewport.height;
		rect.offset.x = 0;
		rect.offset.y = 0;
		
		
		m_currentViewport = viewport;
	}

	VkViewport GetViewport()
	{
		return m_currentViewport;
	}

	void SetGraphicPipelineState(VkPipeline pipeline)
	{
		vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	/*
	void SetPipelineState(PipelineState& pipelineState)
	{
		m_commandList->SetPipelineState(pipelineState.Get());
	}
	
	*/

	/*
	* 
	* /// <summary>
	/// レイトレ用のパイプラインステートオブジェクトを設定。
	/// </summary>
	/// <param name="pso"></param>
	void SetPipelineState(raytracing::PSO& pso);
	*/

	
private:
	VkCommandBuffer m_commandBuffer;
	VkPipeline m_pipeline;
	VkViewport m_currentViewport;
};
