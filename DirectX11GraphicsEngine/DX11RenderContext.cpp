#include "DX11RenderContext.h"

void DX11RenderContext::SetVertexBuffer(ID3D11Buffer* vertexbuffers)
{
	
		m_rendercontext->IASetVertexBuffers(0, 1, &vertexbuffers, 0, 0);//Žb’è“I‚Èˆ’u
		
}

void DX11RenderContext::SetIndexBuffer(ID3D11Buffer* indexbuffer)
{
	m_rendercontext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R8_UINT, 0);//Žb’è“I‚Èˆ’u
}

void DX11RenderContext::SetViewPorts(UINT numviewports,const D3D11_VIEWPORT* viewports)
{
	m_rendercontext->RSSetViewports(numviewports,viewports);
}

void DX11RenderContext::SetRenderTargets(UINT numviews, ID3D11RenderTargetView* rendertargetviews,ID3D11DepthStencilView* depthstencilviews)
{
	m_rendercontext->OMSetRenderTargets(numviews,&rendertargetviews,depthstencilviews);
}

void DX11RenderContext::SetVertexShader(ID3D11VertexShader* vertexShader, UINT numclassInstances)
{
	m_rendercontext->VSSetShader(vertexShader,nullptr,numclassInstances);
}

void DX11RenderContext::SetPixelShader(ID3D11PixelShader* pixelShader, UINT numclassInstances)
{
	m_rendercontext->PSSetShader(pixelShader,nullptr,numclassInstances);

}

void DX11RenderContext::SetComputeShader(ID3D11ComputeShader* computeShader, UINT numclassInstances)
{
	
	m_rendercontext->CSSetShader(computeShader, nullptr, numclassInstances);
}

void DX11RenderContext::SetHullShader(ID3D11HullShader* hullShader, UINT numclassInstances)
{
	m_rendercontext->HSSetShader(hullShader, nullptr, numclassInstances);
}

void DX11RenderContext::SetDomainShader(ID3D11DomainShader* DomainShader, UINT numclassInstances)
{
	m_rendercontext->DSSetShader(DomainShader, nullptr, numclassInstances);
}

void DX11RenderContext::DrawIndexed(UINT indexCount)
{
	m_rendercontext->DrawIndexed(indexCount,0,0);
	
}

void DX11RenderContext::DrawIndexedInstanced(UINT indexcountperinstance,UINT instanceCount)
{
	m_rendercontext->DrawIndexedInstanced(indexcountperinstance,instanceCount,0,0,0);
}

void DX11RenderContext::DrawIndexedInstancedIndirect(ID3D11Buffer* args,UINT AlingedByteOffset) {
	m_rendercontext->DrawIndexedInstancedIndirect(args,AlingedByteOffset);

}