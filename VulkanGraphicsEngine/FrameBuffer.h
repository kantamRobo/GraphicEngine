#pragma once
class FrameBuffer
{

	//�f�X�g���N�^
	~FrameBuffer();
	unsigned int getTextureMemoryTypeIndex(unsigned int requestBits, VkMemoryPropertyFlags requestProps, VkPhysicalDeviceMemoryProperties physMemProps);
	bool CreateFrameBuffer(int w, int h, int mipLevel, VkFormat colorformat, VkFormat depthstencilFormat, float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps);
	//�t���[���o�b�t�@�̍쐬
	//�t���[���o�b�t�@�̕�
	//�t���[���o�b�t�@�̍���
	//�~�b�v�}�b�v���x��
		/// <param name="mipLevel">�~�b�v�}�b�v���x���B0���w�肵���ꍇ�̓~�b�v�}�b�v���T�|�[�g����Ă���GPU�ł�1*1�s�N�Z���܂ł̃~�b�v�}�b�v���쐬�����B</param>
	//�e�N�X�`���z��̃T�C�Y
	//�J���[�o�b�t�@�̃t�H�[�}�b�g
	//�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	bool CreateFramebufferTexture(VulkanGraphicsEngine& ge, VkDevice Device, int w,
		int h, int miplevel,
		 float clearColor[4],
		VkPhysicalDeviceMemoryProperties physMemProps);
	bool CreateImageView(VulkanGraphicsEngine& ge, VkDevice device);
	VulkanTexture& GetRendarTargetTexture()
	{
		return m_FramebufferTexture;
	}



	bool CreateDepthStencilTexture(VulkanGraphicsEngine& ge,
		const VkDevice& Device,
		int w,
		int h,
		VkFormat format);


	//�f�v�X�X�e���V���o�b�t�@�����݂��Ă��邩����
	bool IsExistDepthStencilBuffer()const
	{
		return m_depthStencilTexture;
	}

	int GetWidth()const
	{
		return m_width;
	}

	int GetHeight()const
	{
		return m_height;
	}
	VkFormat GetColorBufferFormat()const
	{
		return m_FramebufferTexture.GetFormat();
	}


	float GetDSVClearValue()const
	{
		return m_dsvClearValue;
	}

private:

	bool CreateDescriptorPool(VulkanGraphicsEngine& ge, VkDevice device);
	bool CreateFramebufferTexture(VulkanGraphicsEngine& ge, VkDevice Device, int w, int h, int miplevel, int arraySize, VkFormat format, float clearColor[4], VkPhysicalDeviceMemoryProperties physMemProps);	//�����_�����O�[�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬
	//�O���t�B�b�N�G���W��
	//�f�o�C�X
	//�e�N�X�`���̕�
	//�e�N�X�`���̍���
	//�~�b�v�}�b�v���x��
	//�e�N�X�`���z��̃T�C�Y
	//�e�N�X�`���̃t�H�[�}�b�g
	//true���Ԃ��Ă����琬��


	/// <summary>
	/// �[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�����쐬�B
	/// </summary>
	/// <param name="ge">�O���t�B�b�N�G���W��</param>
	/// <param name="d3dDevice">D3D�f�o�C�X</param>
	/// <param name="w">�e�N�X�`���̕�</param>
	/// <param name="h">�e�N�X�`���̍���</param>
	/// <param name="format">�e�N�X�`���̃t�H�[�}�b�g</param>
	/// <returns>true���Ԃ��Ă����琬��</returns>
	bool CreateDepthStencilTexture(
		VulkanGraphicsEngine& ge,
		const VkDevice& d3dDevice,
		int w,
		int h,
		VkFormat format);

	
private:
	VulkanTexture m_FramebufferTexture;
	VkBuffer m_framebuffertextureVulkan = nullptr;
	VkBuffer m_depthStencilTexture = nullptr;
	VkImage m_depthImage = nullptr;
	VkImage m_FrameBufferImage = nullptr;
	VkDescriptorPool m_FrameBuffertextureDP = nullptr;
	VkDescriptorPool m_depthstenciltextureDP = nullptr;
	VkDevice m_device;
	/*
		UINT m_rtvDescriptorSize = 0;						//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT m_dsvDescriptorSize = 0;						//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	*/
	int m_width = 0;
	int m_height = 0;
	float m_framebufferClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	float m_dsvClearValue = 1.0f;
	VkDeviceMemory FrameBufferdevicememory;
};

