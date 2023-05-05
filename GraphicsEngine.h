#pragma once
#include "stdafx.h"

#include "RenderContext.h"
#include "D:/gamedevelopment_3DCGAPI_GameEngine/DirectX/DirectX12/Rasterizer/hlsl-grimoire-sample/MiniEngine/DirectXTK/Inc/GraphicsMemory.h"
#include "D:/gamedevelopment_3DCGAPI_GameEngine/DirectX/DirectX12/Rasterizer/hlsl-grimoire-sample/MiniEngine/NullTextureMaps.h"
#include <dxgi1_4.h>
class GraphicsEngine
{
public:
	//デストラクタ
	~GraphicsEngine();
	ComPtr<IDXGIFactory4> CreateDXGIFactory();


	/// <summary>
	/// D3Dデバイスの作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateD3DDevice(ComPtr<IDXGIFactory4> dxgiFactory);
	/// <summary>
	/// コマンドキューの作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateCommandQueue();
	
	/// <summary>
	/// スワップチェインの作成
	/// </summary>
	/// <param name="hwnd">Windowハンドル</param>
	/// <param name="frameBufferWidth">フレームバッファの幅</param>
	/// <param name="frameBufferHeight">フレームバッファの高さ</param>
	/// <param name="dxgiFactory">DirectX グラフィックス インフラストラクチャー</param>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateSwapChain(
		HWND hwnd,
		UINT frameBufferWidth,
		UINT frameBufferHeight,
		IDXGIFactory4* dxgiFactory
	);
	
	
	//初期化
		/// 本関数を呼び出すことでDirectX12の初期化が行われます。
	
	
	
	bool InitGraphicsEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

	//レンダリング開始

	//1フレームのレンダリングの開始時に呼び出す
	void BeginRender();

	//レンダリング終了

	//1フレームのレンダリングの終了時に呼び出す

	void EndRender();

	//D3Dデバイスを取得


	ComPtr<ID3D12Device5> GetD3DDevice()
	{
		return m_d3dDevice;
	}

	//バックバッファの番号を取得
	UINT m_frameIndex;
	//バックバッファの番号
	UINT GetBackBufferIndex()const
	{
		return m_frameIndex;
	}

	//コマンドキューを取得
	ComPtr<ID3D12CommandQueue> GetCommandQueue()const
	{
		return m_commandQueue;
	}
	ComPtr<ID3D12GraphicsCommandList4> GetCommandList()const
	{
		return m_commandList;
	}
	/// <summary>
	/// CBR_SRVのディスクリプタのサイズを取得。
	/// </summary>
	/// <returns></returns>
	/// 
	UINT GetCbrSrvDescriptorSize()const
	{
		return m_cbrSrvDescriptorSize;
	}


	//サンプラのディスクリプタヒープサイズを取得
	UINT GetSamplerDescriptorSize()const
	{
		return m_samplerDescriptorSize;
	}




	//レンダリングコンテキストを取得
	std::shared_ptr<RenderContext> GetRenderContext()
	{
		return m_renderContext;
	}

	//フレームバッファの幅を取得
	/// <summary>
	/// フレームバッファの幅を取得。
	/// </summary>
	/// <returns>フレームバッファの幅。</returns>
	UINT GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	/// <summary>
	/// フレームバッファの高さを取得。
	/// </summary>
	/// <returns>フレームバッファの高さ。</returns>
	UINT GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}

	/// <summary>
	/// レンダリングターゲットをフレームバッファに変更する。
	/// </summary>
	/// <param name="rc"></param>
	void ChangeRenderTargetToFrameBuffer(std::shared_ptr<RenderContext> rc);

	/// <summary>
		/// 現在のフレームバッファのレンダリングターゲットビューを取得。
		/// </summary>
		/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerRTV() const
	{
		return m_currentFrameBufferRTVHandle;
	}

	/// <summary>
	/// フレームバッファにコピー。
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

	//ヌルテクスチャマップを取得


	const std::shared_ptr<NullTextureMaps>GetNullTextureMaps() const
	{
		return m_nullTextureMaps;
	}
	private:
		//GPUベンダー定義。
		enum GPU_Vender {
			GPU_VenderNvidia,	//NVIDIA
			GPU_VenderAMD,		//Intel
			GPU_VenderIntel,	//AMD
			Num_GPUVender,
		};

public:
	enum { FRAME_BUFFER_COUNT = 2 };
private:
	std::shared_ptr<GraphicsEngine> m_graphicsEngine = nullptr;	//グラフィックスエンジン
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Device5> m_d3dDevice;
	UINT m_cbrSrvDescriptorSize = 0;
	UINT m_samplerDescriptorSize = 0;
	UINT m_frameBufferWidth = 0;				//フレームバッファの幅。
	UINT m_frameBufferHeight = 0;
	int m_currentBackBufferIndex = 0;
	std::shared_ptr<RenderContext> m_renderContext;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;		//現在書き込み中のフレームバッファのレンダリングターゲットビューのハンドル。
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;		//現在書き込み中のフレームバッファの深度ステンシルビューの
	ComPtr<ID3D12Resource> m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };	//フレームバッファ用のレンダリングターゲット。
	std::shared_ptr<NullTextureMaps> m_nullTextureMaps;
	std::unique_ptr<DirectX::GraphicsMemory> m_directXTKGfxMemory;
	ComPtr<IDXGISwapChain3> m_swapChain = nullptr;
};

