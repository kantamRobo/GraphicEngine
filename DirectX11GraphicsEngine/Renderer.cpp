#include "Renderer.h"

void Renderer::Init(HWND hwnd, const Microsoft::glTF::Document& doc, const std::string& attrName)
{
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV(640,480);
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	rasterizerState = std::make_shared<DX11RasterizerState>();
	rasterizerState->InitRasterizerState(graphicEngine.m_device.Get(), graphicEngine.m_deviceContext.Get());
	temp_shader = std::make_shared<DX11Shader>();
	temp_shader->InitVertexShader(graphicEngine.m_device.Get(), L"hoge");
	temp_shader->InitPixelShader(graphicEngine.m_device.Get(), L"hoge");
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
	temp_shader->InitLayout(graphicEngine.m_device.Get(), layout);
	model.Init(graphicEngine.m_device.Get(), doc, attrName);
}

void Renderer::Tick()
{
	context->SetVertexBuffer(model.m_mesh->m_vertexbuffer->m_vertexbuffer.Get());
	context->SetIndexBuffer(model.m_mesh->m_indexbuffer->m_IndexBuffer.Get());
	//シェーダーは暫定的なもの。
	context->SetVertexShader(temp_shader->m_VS.Get(), 1);
	context->SetPixelShader(temp_shader->m_PS.Get(), 1);
	context->SetRasterizerState(rasterizerState->m_rasterizerstate.Get());
	context->SetSingleViewPort(&graphicEngine.m_viewport);
	context->SetSingleRenderTarget(graphicEngine.m_backBufferView.Get(),graphicEngine.m_DepthstencilView.Get());
	
	context->DrawIndexedInstanced()
}
