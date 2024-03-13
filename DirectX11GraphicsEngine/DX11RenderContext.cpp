#include "DX11RenderContext.h"

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