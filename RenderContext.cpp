#include "RenderContext.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
void RenderContext::SetDescriptorHeap(ComPtr<DescriptorHeap> descHeap)
{
	m_descriptorHeaps[0] = descHeap.Get();
	m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps.data()->GetAddressOf());
	//�f�B�X�N���v�^�e�[�u���ɓo�^����B
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

void RenderContext::SetRenderTargetAndViewport(std::shared_ptr<RenderTarget> renderTarget)
{
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width =
		static_cast<float>(renderTarget->GetWidth());
	viewport.Height =
		static_cast<float>(renderTarget->GetHeight());
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;
	SetViewportAndScissor(viewport);

	SetRenderTarget(renderTarget);


}

void RenderContext::SetRenderTargetAndViewport(RenderTarget& renderTarget)
{
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(renderTarget.GetWidth());
	viewport.Height = static_cast<float>(renderTarget.GetHeight());
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;
	SetViewportAndScissor(viewport);

	SetRenderTarget(renderTarget);
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
		//�[�x�o�b�t�@���Ȃ��B
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
	}
}

void RenderContext::ClearRenderTargetViews(int numRt, RenderTarget* renderTargets[])
{
	if (renderTargets[0]->IsExistDepthStencilBuffer()) {
		//�[�x�o�b�t�@������B
		ClearDepthStencilView(renderTargets[0]->GetDSVCpuDescriptorHandle(), renderTargets[0]->GetDSVClearValue());
	}
	for (int i = 0; i < numRt; i++) {
		ClearRenderTargetView(renderTargets[i]->GetRTVCpuDescriptorHandle(), renderTargets[i]->GetRTVClearColor());
	}

}

void RenderContext::SetRenderTargets(UINT numRT, RenderTarget* renderTargets[])
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtDSHandleTbl[32];

	int rtNo = 0;

	for (UINT rtNo = 0; rtNo < numRT; rtNo++)
	{
		rtDSHandleTbl[rtNo] = renderTargets[rtNo]
			->GetRTVCpuDescriptorHandle();
	}
	if (renderTargets[0]->IsExistDepthStencilBuffer())
	{
		//�[�x�o�b�t�@������B
		D3D12_CPU_DESCRIPTOR_HANDLE dsDS = renderTargets[0]->GetDSVCpuDescriptorHandle();
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, &dsDS);
	}
	else
	{
		//�[�x�o�b�t�@���Ȃ�
		m_commandList->OMSetRenderTargets(numRT, rtDSHandleTbl, FALSE, nullptr);
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
