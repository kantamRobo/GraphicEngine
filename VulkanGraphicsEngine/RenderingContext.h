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
	void SetIndexBuffer(Index)

private:
	VkCommandBuffer m_commandBuffer;
};
