#pragma once
#include "stdafx.h"
class RenderTarget
{
public:

	~RenderTarget();

	bool CreateRenderTarget(
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT colorFormat,
		DXGI_FORMAT depthStencilFormat,
		float clearColor[4] = nullptr
	);
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����擾�B
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<Texture> GetRenderTargetTexture()
	{
		return m_renderTargetTexture;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
	{
		return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// CPU���̃f�v�X�X�e���V���o�b�t�@�̃f�B�X�N���v�^�n���h�����擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
	{
		return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}
	Texture& GetRenderTargetTexture()
	{
		return m_renderTargetTexture;
	}
	bool IsExistDepthStencilBuffer()const
	{
		return m_depthStencilTexture;
	}

	int GetWidth()const
	{
		return m_width;
	}

	//�����_�����O�^�[�Q�b�g�̍������擾

	int GetHeight()const
	{
		return m_height;
	}

	//�J���[�o�b�t�@�̃t�H�[�}�b�g���擾
	DXGI_FORMAT GetColorBufferFormat()const
	{
		return m_renderTargetTexture.GetFormat();
	}

	const float* GetRTVClearColor()const
	{
		return m_rtvClearColor;
	}

	float GetDSVClearValue()const
	{
		return m_dsvClearValue;
	}

private:
	//�f�B�X�N���v�^�q�[�v���쐬

	bool CreateDescriptorHeap(std::shared_ptr<GraphicEngine> ge, ID3D12Device5* d3dDevice);
		/// <summary>
		/// �����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬�B
		/// </summary>
		/// <param name="ge">�O���t�B�b�N�G���W��</param>
		/// <param name="d3dDevice">D3D�f�o�C�X</param>
		/// <param name="w">�e�N�X�`���̕�</param>
		/// <param name="h">�e�N�X�`���̍���</param>
		/// <param name="mipLevel">�~�b�v�}�b�v���x��</param>
		/// <param name="arraySize">�e�N�X�`���z��̃T�C�Y</param>
		/// <param name="format">�e�N�X�`���̃t�H�[�}�b�g</param>
		/// <returns>true���Ԃ��Ă����琬���B</returns>

		bool CreateRenderTargetTexture(GraphicsEngine& ge,
			ID3D12Device5*& d3dDevice,
			int w,
			int h,
			int mipLevel,
			int arraySize,
			DXGI_FORMAT format,
			float clearColor[4]);

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
		GraphicsEngine& ge,
		ID3D12Device5*& d3dDevice,
		int w,
		int h,
		DXGI_FORMAT format);

	void CreateDescriptor(ComPtr<ID3D12Device5> d3dDevice);

	private:
		std::shared_ptr<Texture> m_renderTargetTexture;
		ComPtr<ID3D12Resource> m_renderTargetTextureDx12 = nullptr;
		ComPtr<ID3D12Resource> m_depthStencilTexture = nullptr;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;
		ComPtr<ID3D12DescriptorHeap> m_dsvHeap = nullptr;
		UINT m_rtvDescriptorSize = 0;
		UINT m_dsvDescriptorSize = 0;
		int m_width = 0;
		int m_height = 0;
		float m_rtvClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	//�����_�����O�^�[�Q�b�g�r���[�̃N���A�J���[�B
		float m_dsvClearValue = 1.0f;
		
};


