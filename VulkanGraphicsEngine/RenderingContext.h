#pragma once
class RenderingContext
{//2023.6.7�ڕW
//�S����
	void InitRenderingContext(std::shared_ptr<VkCommandBuffer> commandbuffer)
	{
		m_commandBuffer = commandbuffer;
	}

	//���_�o�b�t�@��ݒ�
	void SetVertexBuffer(std::shared_ptr<VertexBuffer> in_vb)
	{
		//TODO
		vkCmdBindVertexBuffers(m_commandBuffer, );
	}

	//�C���f�b�N�X�o�b�t�@��ݒ�
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
    

	//�r���[�|�[�g�ƃV�U�����O��`���Z�b�g�Őݒ�

	void SetViewportAndScissor(VkViewport& viewport)
	{
		VkRect2D scissorRect = {};
		scissorRect.

	}


	

private:
	VkCommandBuffer m_commandBuffer;
};
