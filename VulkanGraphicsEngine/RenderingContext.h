#pragma once
#include "UniformBuffer.h"
#include "VulkanTexture.h"
#include "FrameBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
class RenderingContext
{//2023.6.7�ڕW
//�S����
	void InitRenderingContext(std::shared_ptr<VkCommandBuffer> commandbuffer)
	{
		m_commandBuffer = *commandbuffer;
	}

	//���_�o�b�t�@��ݒ�
	void SetVertexBuffer(std::shared_ptr<VulkanVertexBuffer> in_vb)
	{
		//TODO
		vkCmdBindVertexBuffers(m_commandBuffer, );
	}

	//�C���f�b�N�X�o�b�t�@��ݒ�
	void SetIndexBuffer(std::shared_ptr<VulkanIndexbuffer> ib)
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
	
	void SetImageResource(int registerNo, std::shared_ptr<VulkanTexture> texture)
	{
		if (registerNo < MAX_IMAGE_RESOURCE)
		{
			m_ImageResources[registerNo] = texture;
		}
		else
		{
			std::abort();
		}

	}

	//�������̃����_�����O�^�[�Q�b�g��ݒ肷��
	void SetFrameBuffers(unsigned int numFB, std::shared_ptr<FrameBuffer> framebuffers[]);

	//�t���[���o�b�t�@���X���b�g�O�ɐݒ肷��
	//�����̊֐��ł̓r���[�|�[�g�̐ݒ���s��Ȃ�
	//���[�U�[���œK�؂ȃr���[�|�[�g���w�肷��K�v������

	void SetFrameBuffer(FrameBuffer& framebuffer)
	{
		FrameBuffer* fbarray[] = { &framebuffer };
		SetFrameBuffer(1, fbarray);

	}

	inline void SetFrameBufferAndViewport(FrameBuffer& framebuffer);

	void SetFramebuffersAndViewport(unsigned int numFB, FrameBuffer* framebuffers[]);

	//�������̃t���[���o�b�t�@���N���A
	//�N���A�J���[�̓t���[���o�b�t�@�̏��������Ɏw�肵���J���[

	void ClearFrameBuffers(int numFB, FrameBuffer* framebuffers[]);

	void BeginRenderPass(VkRenderPassBeginInfo renderPassBI)
	{
		vkCmdBeginRenderPass(m_commandBuffer, &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);
	}
private:
	enum { MAX_DESCRIPTOR_POOL = 4 };	//
	enum { MAX_UNIFORM_BUFFER = 8 };
	enum { MAX_IMAGE_RESOURCE = 16 };	//�V�F�[�_�[���\�[�X�̍ő吔�B����Ȃ��Ȃ����瑝�₵�ĂˁB
	const float MIN_DEPTH = 0.0f;
	const float MAX_DEPTH = 1.0f;
	VkCommandBuffer m_commandBuffer;
	VkPipeline m_pipeline;
	VkViewport m_currentViewport;
	std::shared_ptr<VulkanTexture> m_ImageResources[MAX_IMAGE_RESOURCE];
	VkDescriptorPool m_descriptorpool[MAX_DESCRIPTOR_POOL];
	std::shared_ptr<UniformBuffer> m_uniformBuffers[MAX_UNIFORM_BUFFER];
};
