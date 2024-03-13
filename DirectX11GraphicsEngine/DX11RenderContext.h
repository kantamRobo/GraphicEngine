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


	//shader
	void SetVertexShader(ID3D11VertexShader* vertexShader, UINT numclassInstances);

	void SetPixelShader(ID3D11PixelShader* pixelShader, UINT numclassInstances);
	
	void SetComputeShader(ID3D11ComputeShader* computeShader, UINT numclassInstances);
	void SetHullShader(ID3D11HullShader* hullShader, UINT numclassInstances);

	void SetDomainShader(ID3D11DomainShader* DomainShader, UINT numclassInstances);
	
};
