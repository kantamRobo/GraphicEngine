#pragma once
#include "RenderContext.h"

void RenderContext::SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap)
{
	m_descriptorHeaps[0] = descHeap->Get().Get();
	m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps);

	//ディスクリプタテーブルに登録する
	if (descHeap->IsRegistConstantBuffer())
	{
		SetGraphicsRootDescriptorTable(0, descHeap->GetConstantBufferGpuDescriptorStartHandle());

	}
	if (descHeap->IsRegistShaderResource())
	{
		SetGraphicsRootDescriptorTable(1, descHeap->GetShaderResourceGpuDescriptorStartHandle());
	}

	if (descHeap->IsRegistUavResource()) {
		SetGraphicsRootDescriptorTable(2, descHeap->GetUavResourceGpuDescriptorStartHandle());
	}
}

void RenderContext::SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap)
{
	m_descriptorHeaps[0] = descHeap.Get()->get();

	//ディスクリプタテーブルに登録する。
	if (descHeap->IsRegistConstantBuffer()) {
		SetComputeRootDescriptorTable(0, descHeap->GetConstantBufferGpuDescriptorStartHandle());
	}
	if (descHeap->IsRegistShaderResource()) {
		SetComputeRootDescriptorTable(1, descHeap->GetShaderResourceGpuDescriptorStartHandle());
	}
	if (descHeap->IsRegistUavResource()) {
		SetComputeRootDescriptorTable(2, descHeap->GetUavResourceGpuDescriptorStartHandle());
	}
}

void RenderContext::WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget)
{
	WaitUntilFinishDrawingToRenderTarget(renderTarget->GetRenderTargetTexture().get());
 }

void RenderContext::WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget)
{
	WaitUntilToPossibleSetRenderTarget(renderTarget->GetRenderTargetTexture().get());

}

void RenderContext::WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[])
{
	for (int i = 0; i < numRt; i++) {
		WaitUntilToPossibleSetRenderTarget(*renderTargets[i]);
	}
}

void RenderContext::WaitUntilFinishDrawingToRenderTargets(int numRt, RenderTarget* renderTargets[])
{
	for (int i = 0; i < numRt; i++) {
		WaitUntilFinishDrawingToRenderTarget(*renderTargets[i]);
	}
}

void RenderContext::SetRenderTargets(UINT numRT, RenderTarget* renderTargets[])
{
	//d
	D3D12_CPU_DESCRIPTOR_HANDLE rtDSHandleTbl[32];
	int rtNo = 0;
	for (UINT rtNo = 0; rtNo < numRT; rtNo++) {
		rtDSHandleTbl[rtNo] = renderTargets[rtNo]->GetRTVCpuDescriptorHandle();
	}
	if (renderTargets[0]->IsExsitDepthStencilBuffer()) {
		//深度バッファがある。
		D3D12_CPU_DESCRIPTOR_HANDLE dsDS = renderTargets[0]->GetDSVCpuDescriptorHandle();
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, &dsDS);
	}
	else {
		//深度バッファがない。
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
	}

}

inline void RenderContext::WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[])
{
	for (int i = 0; i < numRt; i++) {
		WaitUntilToPossibleSetRenderTarget(*renderTargets[i]);
	}
}


inline void RenderContext::WaitUntilToPossibleSetRenderTarget(RenderTarget& renderTarget)
{
	WaitUntilToPossibleSetRenderTarget(renderTarget.GetRenderTargetTexture().Get());
}