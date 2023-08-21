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
		VkRect2D scissorRect = {};
		scissorRect.

	}


	

private:
	VkCommandBuffer m_commandBuffer;
};
