#pragma once
#include "stdafx.h"

#include "RenderContext.h"
#include "D:/gamedevelopment_3DCGAPI_GameEngine/DirectX/DirectX12/Rasterizer/hlsl-grimoire-sample/MiniEngine/DirectXTK/Inc/GraphicsMemory.h"
#include "D:/gamedevelopment_3DCGAPI_GameEngine/DirectX/DirectX12/Rasterizer/hlsl-grimoire-sample/MiniEngine/NullTextureMaps.h"
#include <dxgi1_4.h>
class GraphicsEngine
{
public:
	//�f�X�g���N�^
	~GraphicsEngine();
	ComPtr<IDXGIFactory4> CreateDXGIFactory();


	/// <summary>
	/// D3D�f�o�C�X�̍쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateD3DDevice(ComPtr<IDXGIFactory4> dxgiFactory);
	/// <summary>
	/// �R�}���h�L���[�̍쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateCommandQueue();
	
	/// <summary>
	/// �X���b�v�`�F�C���̍쐬
	/// </summary>
	/// <param name="hwnd">Window�n���h��</param>
	/// <param name="frameBufferWidth">�t���[���o�b�t�@�̕�</param>
	/// <param name="frameBufferHeight">�t���[���o�b�t�@�̍���</param>
	/// <param name="dxgiFactory">DirectX �O���t�B�b�N�X �C���t���X�g���N�`���[</param>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateSwapChain(
		HWND hwnd,
		UINT frameBufferWidth,
		UINT frameBufferHeight,
		IDXGIFactory4* dxgiFactory
	);
	
	
	//������
		/// �{�֐����Ăяo�����Ƃ�DirectX12�̏��������s���܂��B
	
	
	
	bool InitGraphicsEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

	//�����_�����O�J�n

	//1�t���[���̃����_�����O�̊J�n���ɌĂяo��
	void BeginRender();

	//�����_�����O�I��

	//1�t���[���̃����_�����O�̏I�����ɌĂяo��

	void EndRender();

	//D3D�f�o�C�X���擾


	ComPtr<ID3D12Device5> GetD3DDevice()
	{
		return m_d3dDevice;
	}

	//�o�b�N�o�b�t�@�̔ԍ����擾
	UINT m_frameIndex;
	//�o�b�N�o�b�t�@�̔ԍ�
	UINT GetBackBufferIndex()const
	{
		return m_frameIndex;
	}

	//�R�}���h�L���[���擾
	ComPtr<ID3D12CommandQueue> GetCommandQueue()const
	{
		return m_commandQueue;
	}
	ComPtr<ID3D12GraphicsCommandList4> GetCommandList()const
	{
		return m_commandList;
	}
	/// <summary>
	/// CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y���擾�B
	/// </summary>
	/// <returns></returns>
	/// 
	UINT GetCbrSrvDescriptorSize()const
	{
		return m_cbrSrvDescriptorSize;
	}


	//�T���v���̃f�B�X�N���v�^�q�[�v�T�C�Y���擾
	UINT GetSamplerDescriptorSize()const
	{
		return m_samplerDescriptorSize;
	}




	//�����_�����O�R���e�L�X�g���擾
	std::shared_ptr<RenderContext> GetRenderContext()
	{
		return m_renderContext;
	}

	//�t���[���o�b�t�@�̕����擾
	/// <summary>
	/// �t���[���o�b�t�@�̕����擾�B
	/// </summary>
	/// <returns>�t���[���o�b�t�@�̕��B</returns>
	UINT GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	/// <summary>
	/// �t���[���o�b�t�@�̍������擾�B
	/// </summary>
	/// <returns>�t���[���o�b�t�@�̍����B</returns>
	UINT GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɕύX����B
	/// </summary>
	/// <param name="rc"></param>
	void ChangeRenderTargetToFrameBuffer(std::shared_ptr<RenderContext> rc);

	/// <summary>
		/// ���݂̃t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[���擾�B
		/// </summary>
		/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerRTV() const
	{
		return m_currentFrameBufferRTVHandle;
	}

	/// <summary>
	/// �t���[���o�b�t�@�ɃR�s�[�B
	/// </summary>
	/// <param name="pDst"></param>
	void CopyToFrameBuffer(std::shared_ptr<RenderContext> rc, ID3D12Resource* pSrc)
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_renderTargets[m_frameIndex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_COPY_DEST);
		rc->ResourceBarrier(barrier);
		rc->CopyResource(m_renderTargets[m_frameIndex].Get(), pSrc);

		D3D12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			m_renderTargets[m_frameIndex].Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		rc->ResourceBarrier(barrier2);
	}

	//�k���e�N�X�`���}�b�v���擾


	const std::shared_ptr<NullTextureMaps>GetNullTextureMaps() const
	{
		return m_nullTextureMaps;
	}
	private:
		//GPU�x���_�[��`�B
		enum GPU_Vender {
			GPU_VenderNvidia,	//NVIDIA
			GPU_VenderAMD,		//Intel
			GPU_VenderIntel,	//AMD
			Num_GPUVender,
		};

public:
	enum { FRAME_BUFFER_COUNT = 2 };
private:
	std::shared_ptr<GraphicsEngine> m_graphicsEngine = nullptr;	//�O���t�B�b�N�X�G���W��
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Device5> m_d3dDevice;
	UINT m_cbrSrvDescriptorSize = 0;
	UINT m_samplerDescriptorSize = 0;
	UINT m_frameBufferWidth = 0;				//�t���[���o�b�t�@�̕��B
	UINT m_frameBufferHeight = 0;
	int m_currentBackBufferIndex = 0;
	std::shared_ptr<RenderContext> m_renderContext;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;		//���ݏ������ݒ��̃t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�̃n���h���B
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;		//���ݏ������ݒ��̃t���[���o�b�t�@�̐[�x�X�e���V���r���[��
	ComPtr<ID3D12Resource> m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };	//�t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�B
	std::shared_ptr<NullTextureMaps> m_nullTextureMaps;
	std::unique_ptr<DirectX::GraphicsMemory> m_directXTKGfxMemory;
	ComPtr<IDXGISwapChain3> m_swapChain = nullptr;
};

