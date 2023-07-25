#pragma once

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
	bool CreateDescriptorHeap(GraphicsEngine* ge, ID3D12Device5* d3dDevice);
	bool CreateRenderTargetTexture(GraphicsEngine* ge, ID3D12Device5* d3dDevice, int w, int h, int mipLevel, int arraySize, DXGI_FORMAT format, float clearColor[4]);
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

	/*
	//カラーバッファのフォーマットを取得
	DXGI_FORMAT GetColorBufferFormat()const
	{
		return m_renderTargetTexture->GetFormat();
	}
	*/

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

	

		

	/// <summary>
	/// 深度ステンシルバッファとなるテクスチャを作成。
	/// </summary>
	/// <param name="ge">グラフィックエンジン</param>
	/// <param name="d3dDevice">D3Dデバイス</param>
	/// <param name="w">テクスチャの幅</param>
	/// <param name="h">テクスチャの高さ</param>
	/// <param name="format">テクスチャのフォーマット</param>
	/// <returns>trueが返ってきたら成功</returns>
	

	

	

	bool CreateDepthStencilTexture(GraphicsEngine* ge, ID3D12Device5* d3dDevice, int w, int h, DXGI_FORMAT format);

	void CreateDescriptor(Microsoft::WRL::ComPtr<ID3D12Device5> d3dDevice);

	private:
		std::shared_ptr<Texture> m_renderTargetTexture;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargetTextureDx12 = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilTexture = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap = nullptr;
		UINT m_rtvDescriptorSize = 0;
		UINT m_dsvDescriptorSize = 0;
		int m_width = 0;
		int m_height = 0;
		float m_rtvClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	//レンダリングターゲットビューのクリアカラー。
		float m_dsvClearValue = 1.0f;
		
};


