#pragma once
#include <d3d11.h>
#include <wrl.h>

//グラフィックスパイプラインに関わる関数を一元管理する(入力アセンブリ・シェーダー・ラスタライザ・アウトプットマージャー,またはコンピュートパイプラインの実行や、それにかかわるコマンド)
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
	void SetSingleViewPort(const D3D11_VIEWPORT* viewports);
	void SetScssciorRects(UINT numRects, D3D11_RECT* rects);

	

	

	void SetRenderTargets(UINT numviews, ID3D11RenderTargetView* rendertargetviews, ID3D11DepthStencilView* depthstencilviews);

	void SetBlendState(ID3D11BlendState* blenderstate);

	

	

	void SetSingleRenderTarget(ID3D11RenderTargetView* rendertargetviews, ID3D11DepthStencilView* depthstencilviews);

	void ClearRenderTargetView(ID3D11RenderTargetView* RenderTargetView, float nextcolor[4]);

	

	void SetStates(UINT numbuffers, ID3D11Buffer* vertexbuffer, UINT offset);

	void SetState(ID3D11Buffer* vertexbuffer);

	

	
	

	

	

	//shader
	void SetVertexShader(ID3D11VertexShader* vertexShader, UINT numclassInstances);

	void SetVertexShader_ConstantBuffer(ID3D11Buffer* constantbuffer);

	
	void SetPixelShader(ID3D11PixelShader* pixelShader, UINT numclassInstances);
	
	void SetComputeShader(ID3D11ComputeShader* computeShader, UINT numclassInstances);
	void SetHullShader(ID3D11HullShader* hullShader, UINT numclassInstances);

	void SetDomainShader(ID3D11DomainShader* DomainShader, UINT numclassInstances);

	void SetRasterizerState(ID3D11RasterizerState* rasterizerstate);

	void SetViewPort(const D3D11_VIEWPORT* viewport);

	
	void DrawIndexed(UINT indexCount);

	void DrawIndexedInstanced(UINT indexcountperinstance, UINT instanceCount);

	void DrawIndexedInstancedIndirect(ID3D11Buffer* GPUPrimitiveBuffer, UINT AlingedByteOffset);

	void DrawDispatchIndirect(ID3D11Buffer* GPUPrimitivebuffer, UINT Offset);

	
	
	

	//Compute Pipeline

	void Dispatch(UINT numthreadX, UINT numthreadY, UINT numthreadZ);

	
	
};

