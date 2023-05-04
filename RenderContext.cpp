#include "RenderContext.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
void RenderContext::SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap)
{
	m_descriptorHeaps[0] = descHeap.Get();
	m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps.data()->GetAddressOf());
	//ディスクリプタテーブルに登録する。
	if (descHeap->IsRegistConstantBuffer()) {
		SetGraphicsRootDescriptorTable(0, 
			descHeap->GetConstantBufferGpuDescriptorStartHandle());
	}
	if (descHeap->IsRegistShaderResource()) {
		SetGraphicsRootDescriptorTable(1, 
			descHeap->GetShaderResourceGpuDescriptorStartHandle());
	}
	if (descHeap->IsRegistUavResource()) {
		SetGraphicsRootDescriptorTable(2, 
			descHeap->GetUavResourceGpuDescriptorStartHandle());
	}
}

void RenderContext::SetComputeDescriptorHeap(ComPtr<DescriptorHeap> descHeap)
{
}

void RenderContext::SetRenderTargetAndViewport(RenderTarget& renderTarget)
{
}

void RenderContext::SetRenderTargetsAndViewport(UINT numRT, RenderTarget* renderTargets[])
{
	//d
	D3D12_CPU_DESCRIPTOR_HANDLE rtDSHandleTbl[32];
	int rtNo = 0;
	for (UINT rtNo = 0; rtNo < numRT; rtNo++)
	{
		rtDSHandleTbl[rtNo] = renderTargets[rtNo]->GetRTVCpuDescriptorHandle();
	}

	if (renderTargets[0]->IsExistDepthStencilBuffer())
	{
		D3D12_CPU_DESCRIPTOR_HANDLE dsDS = renderTargets[0]->GetDSVCpuDescriptorHandle();
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, &dsDS);
	}
	else {
		//深度バッファがない。
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
	}
}

void RenderContext::SetRenderTargets(UINT numRT, RenderTarget* renderTargets[])
{
}

void RenderContext::WaitUntilFinishDrawingToRenderTarget(ComPtr<RenderTarget> renderTarget)
{
	WaitUntilFinishDrawingToRenderTarget(renderTarget->GetRenderTargetTexture().get());
}

void RenderContext::WaitUntilToPossibleSetRenderTarget(ComPtr<RenderTarget> renderTarget)
{
	WaitUntilToPossibleSetRenderTarget(renderTarget->GetRenderTargetTexture().get());
}
