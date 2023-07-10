#pragma once
#include "stdafx.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "PipelineState.h"
class ConstantBuffer;
class Texture;
class DescriptorHeap;
enum { MAX_DESCRIPTOR_HEAP = 4 };	//ディスクリプタヒープの最大数。
enum { MAX_CONSTANT_BUFFER = 8 };	//定数バッファの最大数。足りなくなったら増やしてね。
enum { MAX_SHADER_RESOURCE = 16 };	//シェーダーリソースの最大数。足りなくなったら増やしてね。

class RenderContext
{
public:

	/// <summary>
	/// リソースをコピー。
	/// </summary>
	/// <param name="pDst">コピー先のリソース</param>
	/// <param name="pSrc">コピー元のリソース</param>
	void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc)
	{
		m_commandList->CopyResource(pDst, pSrc);
	}
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

		
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps->GetAddressOf());

	}
	inline void SetDescriptorHeap(DescriptorHeap& descHeap)
	{
		m_descriptorHeaps[0] = descHeap.get();
		m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps->GetAddressOf());

		//ディスクリプタテーブルに登録する。
		if (descHeap.IsRegistConstantBuffer()) {
			SetGraphicsRootDescriptorTable(0, descHeap.GetConstantBufferGpuDescriptorStartHandle());
		}
		if (descHeap.IsRegistShaderResource()) {
			SetGraphicsRootDescriptorTable(1, descHeap.GetShaderResourceGpuDescriptorStartHandle());
		}
		if (descHeap.IsRegistUavResource()) {
			SetGraphicsRootDescriptorTable(2, descHeap.GetUavResourceGpuDescriptorStartHandle());
		}
	}

		void SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap);
		void SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap);

		void SetRenderTargetAndViewport(std::shared_ptr<RenderTarget> renderTarget);

		void SetDescriptorHeaps(int numDescriptorHeap, const ComPtr<DescriptorHeap> descHeaps[])
		{
			for (int i = 0; i < numDescriptorHeap; i++)
			{
				m_descriptorHeaps[i] = descHeaps[i]->get();

			}
			m_commandList->SetDescriptorHeaps(numDescriptorHeap
				, m_descriptorHeaps->GetAddressOf());
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
		/// <param name="renderTarget"></param>
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
		{
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		}



		/// <summary>
		/// レンダリングターゲットをスロット0に設定する。
		/// </summary>
		/// <remarks>
		/// 本関数はビューポートの設定を行いません。
		/// ユーザー側で適切なビューポートを指定する必要があります。
		/// </remarks>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void SetRenderTarget(std::shared_ptr<RenderTarget> Rendertarget)
		{
			std::array<RenderTarget*, 1>rtArray[] = { Rendertarget.get() };
			SetRenderTargets(1, rtArray->data());
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
		void SetRenderTargetAndViewport(RenderTarget & renderTarget);
		/// <summary>
		/// 複数枚のレンダリングターゲットとビューポートを同時に設定する。
		/// </summary>
		/// /// <remarks>
		/// この関数を利用するとレンダリングターゲットと同じ幅と高さのビューポートが設定されます。
		/// </remarks>
		/// <param name="numRT">設定するレンダリングターゲットの枚数</param>
		/// <param name="renderTargets">レンダリングターゲットの配列。</param>
		void SetRenderTargetsAndViewport(UINT numRT, RenderTarget * renderTargets[]);

		/// <summary>
	/// 複数枚のレンダリングターゲットをクリア。
	/// </summary>
	/// <remarks>
	/// クリアカラーはレンダリングターゲットの初期化時に指定したカラーです。
	/// </remarks>
	/// <param name="numRt">レンダリングターゲットの数</param>
	/// <param name="renderTargets">レンダリングターゲットの数</param>
		void ClearRenderTargetViews(
			int numRt,
			RenderTarget* renderTargets[]
		);
		void SetPipelineState(PSO& pso);
		void SetComputeDescriptorHeap(std::shared_ptr<DescriptorHeap> descHeap);
		void WaitUntilFinishDrawingToRenderTarget(std::shared_ptr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTarget(std::shared_ptr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTargets(int numRt, std::shared_ptr<RenderTarget> renderTargets[]);
		void WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[]);
		
		void WaitUntilFinishDrawingToRenderTargets(int numRt, std::shared_ptr<RenderTarget> renderTargets[]);
		/// <summary>
	/// 複数枚のレンダリングターゲットを設定する。
	/// </summary>
	/// <remarks>
	/// MRTを利用したレンダリングを行いたい場合に利用してください。
	/// </remarks>
	/// <param name="numRT">レンダリングターゲットの数</param>
	/// <param name="renderTarget">レンダリングターゲットの配列。</param>
		void SetRenderTargets(UINT numRT, RenderTarget * renderTargets[]);


		void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);
		void WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget);
		void WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget);

		
		void ResourceBarrier(D3D12_RESOURCE_BARRIER & barrier)
		{
			m_commandList->ResourceBarrier(1, &barrier);
		}
		/// <summary>
		/// リソースステートを遷移する。
		/// </summary>
		/// <param name="resrouce"></param>
		/// <param name="beforeState"></param>
		/// <param name="afterState"></param>
		void TransitionResourceState(ID3D12Resource * resrouce, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
		{
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resrouce, beforeState, afterState);
			m_commandList->ResourceBarrier(1, &barrier);
		}
		/// <summary>
		/// コマンドリストを閉じる
		/// </summary>
		void Close()
		{
			m_commandList->Close();
		}

		/// <summary>
		/// インデックスつきプリミティブを描画。
		/// </summary>
		/// <param name="indexCount">インデックスの数。</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}
		/// <summary>
		/// インスタンシング描画
		/// </summary>
		/// <param name="indexCount">インデックス数</param>
		/// <param name="numInstance">インスタンス数</param>
		void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
		{
			m_commandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
		}
		/// <summary>
		/// コンピュートシェーダーをディスパッチ。
		/// </summary>
		/// <param name="ThreadGroupCountX"></param>
		/// <param name="ThreadGroupCountY"></param>
		/// <param name="ThreadGroupCountZ"></param>
		void Dispatch(
			UINT ThreadGroupCountX,
			UINT ThreadGroupCountY,
			UINT ThreadGroupCountZ)
		{
			m_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}




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
	/// レンダリングターゲットのクリア。
	/// </summary>
	/// <param name="rtvHandle">CPUのレンダリングターゲットビューのディスクリプタハンドル</param>
	/// <param name="clearColor">クリアカラー</param>
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
		{
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}

		/// <summary>
	/// デプスステンシルビューをクリア
	/// </summary>
	/// <param name="renderTarget">レンダリングターゲット</param>
	/// <param name="clearValue">クリア値</param>
		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
		{
			m_commandList->ClearDepthStencilView(
				dsvHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearValue,
				0,
				0,
				nullptr);
		}

		/// <summary>
	/// コマンドリストを閉じる
	/// </summary>
		void Close()
		{
			m_commandList->Close();
		}

		/// <summary>
/// コマンドリストをリセット。
/// </summary>
/// <param name="commandAllocator"></param>
/// <param name="pipelineState"></param>
		void Reset(ID3D12CommandAllocator* commandAllocator, ID3D12PipelineState* pipelineState)
		{
			m_commandList->Reset(commandAllocator, pipelineState);

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

		/// <summary>
	/// インデックスつきプリミティブを描画。
	/// </summary>
	/// <param name="indexCount">インデックスの数。</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}

		/// <summary>
	/// インスタンシング描画
	/// </summary>
	/// <param name="indexCount">インデックス数</param>
	/// <param name="numInstance">インスタンス数</param>
		void DrawIndexedInstanced(UINT indexCount, UINT numInstance)
		{
			m_commandList->DrawIndexedInstanced(indexCount, numInstance, 0, 0, 0);
		}

		/// <summary>
	/// コンピュートシェーダーをディスパッチ。
	/// </summary>
	/// <param name="ThreadGroupCountX"></param>
	/// <param name="ThreadGroupCountY"></param>
	/// <param name="ThreadGroupCountZ"></param>
		void Dispatch(
			UINT ThreadGroupCountX,
			UINT ThreadGroupCountY,
			UINT ThreadGroupCountZ)
		{
			m_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}

		/// <summary>
	/// GPUでレイトレーシングアクセラレーション構造のビルドを行います。
	/// </summary>
		void BuildRaytracingAccelerationStructure(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc)
		{
			m_commandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
		}
		/// <summary>
		/// レイをディスパッチ
		/// </summary>
		/// <param name="rayDesc"></param>
		void DispatchRays(D3D12_DISPATCH_RAYS_DESC& rayDesc)
		{
			m_commandList->DispatchRays(&rayDesc);
		}
		/// <summary>
		/// リソースをコピー。
		/// </summary>
		/// <param name="pDst">コピー先のリソース</param>
		/// <param name="pSrc">コピー元のリソース</param>
		void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc)
		{
			m_commandList->CopyResource(pDst, pSrc);
		}
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

		private:
		ComPtr<ID3D12GraphicsCommandList4> m_commandList;
		ComPtr<ID3D12DescriptorHeap> m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];
		std::shared_ptr<ConstantBuffer>m_constantBuffers[MAX_CONSTANT_BUFFER];
		std::shared_ptr<Texture> m_shaderResources[MAX_SHADER_RESOURCE];
		std::vector < ComPtr<ID3D12Resource>> m_scratchResourceList;
		D3D12_VIEWPORT m_currentViewport;
	
};
