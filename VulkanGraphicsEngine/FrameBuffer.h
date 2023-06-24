#pragma once
class FrameBuffer
{

	//�f�X�g���N�^
	~FrameBuffer();
	//�t���[���o�b�t�@�̍쐬
	//�t���[���o�b�t�@�̕�
	//�t���[���o�b�t�@�̍���
	//�~�b�v�}�b�v���x��
		/// <param name="mipLevel">�~�b�v�}�b�v���x���B0���w�肵���ꍇ�̓~�b�v�}�b�v���T�|�[�g����Ă���GPU�ł�1*1�s�N�Z���܂ł̃~�b�v�}�b�v���쐬�����B</param>
	//�e�N�X�`���z��̃T�C�Y
	//�J���[�o�b�t�@�̃t�H�[�}�b�g
	//�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	bool CreateFrameBuffer(int w, 
		int h, 
		int mipLevel, 
		VkFormat colorformat,
		VkFormat depthstencilFormat,
		float clearColor[4] =nullptr);
	VulkanTexture& GetRendarTargetTexture()
	{
		return m_FramebufferTexture;
	}

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

	bool CreateDescriptorHeap(std::shared_ptr<VulkanGraphicsEngine> ge, VkDevice device);
	//�����_�����O�[�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬
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
		VkDevice& d3dDevice,
		int w,
		int h,
		VkFormat format);

	/// <summary>
	/// �f�B�X�N���v�^�̍쐬�B
	/// </summary>
	/// <param name="d3dDevice">D3D�f�o�C�X</param>
	/// <returns>true���Ԃ��Ă��`�A�琬���B</returns>
	void CreateDescriptor(VkDevice& d3dDevice);
private:
	VulkanTexture m_FramebufferTexture;
	VkBuffer m_framebuffertextureVulkan = nullptr;
	VkBuffer m_depthStencilTexture = nullptr;
	/*
		UINT m_rtvDescriptorSize = 0;						//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT m_dsvDescriptorSize = 0;						//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	*/
	int m_width = 0;
	int m_height = 0;
	float m_framebufferClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	float m_dsvClearValue = 1.0f;
	
};

