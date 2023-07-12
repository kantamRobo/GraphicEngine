#include "FrameBuffer.h"

bool FrameBuffer::InitFrameBuffer( CoreGraphicsEngine* in_engine)
{
	m_coreGraphicsengine = in_engine;
   
	//�t���[���o�b�t�@�p�̃e�N�X�`���쐬();

	//�f�v�X�X�e���V���e�N�X�`���̍쐬();

	//�t���[���o�b�t�@�p�̃C���t�H���쐬();


	return false;
}

void FrameBuffer::CreateTextureforFramebuffer(const VkRenderPass& renderpass,unsigned int width,unsigned int height)
{
	VkFramebufferCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	ci.renderPass = m_coreGraphicsengine->m_renderpass;
	ci.width = width;
	ci.height = height;
	ci.layers = 1;

	


	for (auto& v : m_swapchainviews)
	{
		std::array<VkImageView, 2> framebuffers = {};
		ci.attachmentCount = uint32_t(attachments.size());
		ci.pAttachments = attachments.data();
		attachments[0] = v;
		attachments[1] = m_depthBufferView;

		VkFramebuffer framebuffer;
		auto result = vkCreateFramebuffer(m_coreGraphicsengine->Device, &ci, nullptr, &framebuffer);
		checkResult(result);
		m_framebuffers.push_back(framebuffer);


	}
}
