#pragma once
#include "stdafx.h"
#include "PipelineState.h"
#include "RootSignature.h"

class ConstantBuffer;
class Texture;
class DescriptorHeap;
class RenderTarget;
class RenderContext
{
public:
	void InitRenderingContext(ComPtr<ID3D12GraphicsCommandList4> commandList)
	{
		m_commandList = commandList;
	}
	/// <summary>
	/// 頂点バッファを設定。
	/// </summary>
	/// <param name="vb">頂点バッファ。</param>
	void SetVertexBuffer(VertexBuffer& vb)
	{
		m_commandList->IASetVertexBuffers(0, 1, &vb.GetView());
	}
	/// <summary>
	/// インデックスバッファを設定。
	/// </summary>
	/// <param name="ib"></param>
	void SetIndexBuffer(IndexBuffer& ib)
	{
		m_commandList->IASetIndexBuffer(&ib.GetView());
	}
	/// <summary>
	/// プリミティブのトポロジーを設定。
	/// </summary>
	/// <remarks>
	/// ID3D12GraphicsCommandList::のIASetPrimitiveTopologyのラッパー関数。
	/// 詳細はMicrosoftのヘルプを参照。
	/// </remarks>
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		m_commandList->IASetPrimitiveTopology(topology);
	}
	/// <summary>
	/// コマンドリストを設定。
	/// </summary>
	/// <param name="commandList">コマンドリスト。</param>
	void SetCommandList(ID3D12GraphicsCommandList4* commandList)
	{
		m_commandList = commandList;
	}
	/// <summary>
	/// ビューポートとシザリング矩形をセットで設定
	/// </summary>
	/// <param name="viewport">ビューポート</param>
	void SetViewportAndScissor(D3D12_VIEWPORT& viewport)
	{
		//シザリング矩形も設定する。
		D3D12_RECT scissorRect;
		scissorRect.bottom = static_cast<LONG>(viewport.Height);
		scissorRect.top = 0;
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(viewport.Width);
		SetScissorRect(scissorRect);

		m_commandList->RSSetViewports(1, &viewport);
		m_currentViewport = viewport;
	}
	/// <summary>
	/// ビューポートを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_VIEWPORT GetViewport() const
	{
		return m_currentViewport;
	}
	/// <summary>
	/// シザリング矩形を設定
	/// </summary>
	/// <param name="rect"></param>
	void SetScissorRect(D3D12_RECT& rect)
	{
		m_commandList->RSSetScissorRects(1, &rect);
	}

	/// <summary>
	/// ルートシグネチャを設定。
	/// </summary>
	void SetRootSignature(ID3D12RootSignature* rootSignature)
	{
		m_commandList->SetGraphicsRootSignature(rootSignature);
	}
	void SetRootSignature(std::shared_ptr<RootSignature> rootSignature)
	{
		m_commandList->SetGraphicsRootSignature(rootSignature->Get().Get());
	}
	void SetComputeRootSignature(ID3D12RootSignature* rootSignature)
	{
		m_commandList->SetComputeRootSignature(rootSignature);
	}
	void SetComputeRootSignature(std::shared_ptr<RootSignature> rootSignature)
	{
		m_commandList->SetComputeRootSignature(rootSignature->Get().Get());
	}
	/// <summary>
	/// パイプラインステートを設定。
	/// </summary>
	void SetPipelineState(ID3D12PipelineState* pipelineState)
	{
		m_commandList->SetPipelineState(pipelineState);
	}
	void SetPipelineState(std::shared_ptr<PipelineState> pipelineState)
	{
		m_commandList->SetPipelineState(pipelineState->Get().Get());
	}
	
	/// <summary>
	/// ディスクリプタヒープを設定。
	/// </summary>
	void SetDescriptorHeap(ID3D12DescriptorHeap* descHeap)
	{
		m_descriptorHeaps[0] = descHeap;
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps.data()->GetAddressOf());
	}

	void SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap);
	void SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap);

	void SetDescriptorHeaps(int numDescriptorHeap, const ComPtr<DescriptorHeap> descHeaps[])
	{
		for (int i = 0; i < numDescriptorHeap; i++)
		{
			m_descriptorHeaps[i] = descHeaps[i];

		}
		m_commandList->SetDescriptorHeaps(numDescriptorHeap
			, m_descriptorHeaps.data()->GetAddressOf());
	}

	void SetConstantBuffer(int registerNo, std::shared_ptr<ConstantBuffer> cb)
	{
		if (registerNo < MAX_CONSTANT_BUFFER) {
			m_constantBuffers[registerNo] = cb;
		}
		else {
			//範囲外アクセス。
			std::abort();
		}
	}

	void SetShaderResource(int registerNo, std::shared_ptr<Texture> texture)
	{

	}

	/// <summary>
	/// レンダリングターゲットとビューポートを同時に設定する。
	/// </summary>
	/// <remarks>
	/// この関数を利用するとレンダリングターゲットと同じ幅と高さのビューポートが設定されます。
	/// </remarks>
	/// <param name="renderTarget">レンダリングターゲット</param>
	void SetRenderTargetAndViewport(RenderTarget& renderTarget);
	/// <summary>
	/// 複数枚のレンダリングターゲットとビューポートを同時に設定する。
	/// </summary>
	/// /// <remarks>
	/// この関数を利用するとレンダリングターゲットと同じ幅と高さのビューポートが設定されます。
	/// </remarks>
	/// <param name="numRT">設定するレンダリングターゲットの枚数</param>
	/// <param name="renderTargets">レンダリングターゲットの配列。</param>
	void SetRenderTargetsAndViewport(UINT numRT, RenderTarget* renderTargets[]);


	/// <summary>
/// 複数枚のレンダリングターゲットを設定する。
/// </summary>
/// <remarks>
/// MRTを利用したレンダリングを行いたい場合に利用してください。
/// </remarks>
/// <param name="numRT">レンダリングターゲットの数</param>
/// <param name="renderTarget">レンダリングターゲットの配列。</param>
	void SetRenderTargets(UINT numRT, RenderTarget* renderTargets[]);


	void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
	void WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget);
	void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
private:

	/// <summary>
	/// ディスクリプタテーブルを設定。
	/// </summary>
	/// <param name="RootParameterIndex"></param>
	/// <param name="BaseDescriptor"></param>
	void SetGraphicsRootDescriptorTable(
		UINT RootParameterIndex,
		D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
	{
		m_commandList->SetGraphicsRootDescriptorTable(
			RootParameterIndex,
			BaseDescriptor
		);
	}
	/// <summary>
	/// ディスクリプタテーブルを設定。
	/// </summary>
	/// <param name="RootParameterIndex"></param>
	/// <param name="BaseDescriptor"></param>
	void SetComputeRootDescriptorTable(
		UINT RootParameterIndex,
		D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
	{
		m_commandList->SetComputeRootDescriptorTable(
			RootParameterIndex,
			BaseDescriptor
		);
	}

	

	enum { MAX_DESCRIPTOR_HEAP = 4 };
	enum { MAX_CONSTANT_BUFFER = 8 };
	enum { MAX_SHADER_RESOURCE = 16 };
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	std::array<ComPtr<ID3D12DescriptorHeap>, MAX_DESCRIPTOR_HEAP> m_descriptorHeaps;
	std::shared_ptr<ConstantBuffer>m_constantBuffers[MAX_CONSTANT_BUFFER];
	std::shared_ptr<Texture> m_shaderResources[MAX_SHADER_RESOURCE];

};

