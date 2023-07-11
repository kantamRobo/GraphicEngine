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
	/// レンダリングターゲットとなるテクスチャを取得。
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
	/// CPU側のデプスステンシルバッファのディスクリプタハンドルを取得。
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

	//レンダリングターゲットの高さを取得

	int GetHeight()const
	{
		return m_height;
	}

	//カラーバッファのフォーマットを取得
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
	//ディスクリプタヒープを作成

	bool CreateDescriptorHeap(std::shared_ptr<GraphicEngine> ge, ID3D12Device5* d3dDevice);
		/// <summary>
		/// レンダリングターゲットとなるテクスチャを作成。
		/// </summary>
		/// <param name="ge">グラフィックエンジン</param>
		/// <param name="d3dDevice">D3Dデバイス</param>
		/// <param name="w">テクスチャの幅</param>
		/// <param name="h">テクスチャの高さ</param>
		/// <param name="mipLevel">ミップマップレベル</param>
		/// <param name="arraySize">テクスチャ配列のサイズ</param>
		/// <param name="format">テクスチャのフォーマット</param>
		/// <returns>trueが返ってきたら成功。</returns>

		bool CreateRenderTargetTexture(GraphicsEngine& ge,
			ID3D12Device5*& d3dDevice,
			int w,
			int h,
			int mipLevel,
			int arraySize,
			DXGI_FORMAT format,
			float clearColor[4]);

	/// <summary>
	/// 深度ステンシルバッファとなるテクスチャを作成。
	/// </summary>
	/// <param name="ge">グラフィックエンジン</param>
	/// <param name="d3dDevice">D3Dデバイス</param>
	/// <param name="w">テクスチャの幅</param>
	/// <param name="h">テクスチャの高さ</param>
	/// <param name="format">テクスチャのフォーマット</param>
	/// <returns>trueが返ってきたら成功</returns>
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
		float m_rtvClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	//レンダリングターゲットビューのクリアカラー。
		float m_dsvClearValue = 1.0f;
		
};


