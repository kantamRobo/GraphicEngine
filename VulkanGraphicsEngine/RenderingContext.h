#pragma once
#include "UniformBuffer.h"
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
	/// ���C�g���p�̃p�C�v���C���X�e�[�g�I�u�W�F�N�g��ݒ�B
	/// </summary>
	/// <param name="pso"></param>
	void SetPipelineState(raytracing::PSO& pso);
	*/

	void SetDescriptorPool(VkDescriptorPool pool)
	{
		m_descriptorpool[0] = pool;
		
	}
	//void SetDescriptorHeap(DescriptorHeap& descHeap);
	

	//���j�t�H�[���o�b�t�@��ݒ�
	void SetUniformBuffer(int registerNo, std::shared_ptr<UniformBuffer> ub)
	{
		if (registerNo < MAX_UNIFORM_BUFFER)
		{
			m_uniformBuffers[registerNo] = ub;
		}
		else
		{

		}
	}
	
	void SetImageResource(int registerNo, Texture& texture)
	{


	}
private:
	enum { MAX_DESCRIPTOR_POOL = 4 };	//
	enum { MAX_UNIFORM_BUFFER = 8 };
	VkCommandBuffer m_commandBuffer;
	VkPipeline m_pipeline;
	VkViewport m_currentViewport;
	VkDescriptorPool m_descriptorpool[MAX_DESCRIPTOR_POOL];
	std::shared_ptr<UniformBuffer> m_uniformBuffers[MAX_UNIFORM_BUFFER];
};
