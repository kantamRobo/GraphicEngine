#pragma once
#include <d3d11.h>
#include <wrl.h>
class DX11RenderContext
{
public:

	DX11RenderContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> in_rendercontext)
	{
		m_rendercontext = in_rendercontext;


		

		
		
		
	}
	
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_rendercontext;

	void SetVertexBuffer(ID3D11Buffer* vertexbuffers);

	void SetIndexBuffer(ID3D11Buffer* indexbuffer);

	void SetViewPorts(UINT numviewports, const D3D11_VIEWPORT* viewports);

	void SetRenderTargets(UINT numviews, ID3D11RenderTargetView* rendertargetviews, ID3D11DepthStencilView* depthstencilviews);

	
	

	

	

	//shader
	void SetVertexShader(ID3D11VertexShader* vertexShader, UINT numclassInstances);

	void SetPixelShader(ID3D11PixelShader* pixelShader, UINT numclassInstances);
	
	void SetComputeShader(ID3D11ComputeShader* computeShader, UINT numclassInstances);
	void SetHullShader(ID3D11HullShader* hullShader, UINT numclassInstances);

	void SetDomainShader(ID3D11DomainShader* DomainShader, UINT numclassInstances);
	
};

