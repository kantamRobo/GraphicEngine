#include "Vertex.h"
#include "SceneConstant.h"
#include "Renderer.h"


std::vector<UINT> g_IndexList{
		0,  1,  2,     3,  2,  1,
		4,  5,  6,     7,  6,  5,
		8,  9, 10,    11, 10,  9,
	   12, 13, 14,    15, 14, 13,
	   16, 17, 18,    19, 18, 17,
	   20, 21, 22,    23, 22, 21,
};
//const Microsoft::glTF::Document& doc, const std::string& attrName
void Renderer::Init(HWND hwnd)
{


	
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV();


	model = std::make_shared<DX11Model>(graphicEngine.m_device.Get(),"alicia-solid.vrm");

		
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	rasterizerState = std::make_shared<DX11RasterizerState>();
	rasterizerState->InitRasterizerState(&graphicEngine, graphicEngine.m_deviceContext.Get(),&graphicEngine.m_viewport);
	temp_shader = std::make_shared<DX11Shader>();
	temp_shader->InitVertexShader(graphicEngine.m_device.Get(), L"VertexShader.hlsl");
	temp_shader->InitPixelShader(graphicEngine.m_device.Get(), L"PixelShader.hlsl");
	



	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(2.0f, 2.0f, -2.0f, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

	float    fov = DirectX::XMConvertToRadians(45.0f);
	float    aspect = graphicEngine.m_viewport.Width / graphicEngine.m_viewport.Height;
	float    nearZ = 0.1f;
	float    farZ = 100.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovRH(fov, aspect, nearZ, farZ);


	DirectX::XMStoreFloat4x4(&temp_const.world, XMMatrixTranspose(worldMatrix));
	DirectX::XMStoreFloat4x4(&temp_const.view, XMMatrixTranspose(viewMatrix));
	DirectX::XMStoreFloat4x4(&temp_const.proj, XMMatrixTranspose(projMatrix));

	graphicEngine.m_deviceContext->UpdateSubresource(m_constbuffer.m_constantBuffer.Get(), 0, NULL, &temp_const, 0, 0);
}

void Renderer::Tick()
{
	context->SetVertexBuffer(model->m_mesh->m_vertexbuffer->m_vertexbuffer.Get());
	context->SetIndexBuffer(model->m_mesh->m_indexbuffer->m_IndexBuffer.Get());
	context->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//シェーダーは暫定的なもの。
	context->SetInputLayout(temp_shader->m_InputLayout.Get());
	context->SetVertexShader(temp_shader->m_VS.Get(), 1);
	context->SetVertexShader_SingleConstantBuffer(m_constbuffer.m_constantBuffer.Get());
	
	
	context->SetPixelShader(temp_shader->m_PS.Get(), 1);
	context->SetRasterizerState(rasterizerState->m_rasterizerstate.Get());
	context->SetSingleViewPort(&graphicEngine.m_viewport);
	context->SetSingleRenderTarget(graphicEngine.m_backBufferView.Get(),graphicEngine.m_DepthstencilView.Get());
	
	context->DrawIndexed(g_IndexList.size());
}
