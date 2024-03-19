#include "Renderer.h"

void Renderer::Init(HWND hwnd, const Microsoft::glTF::Document& doc, const std::string& attrName)
{
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV(640,480);
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	rasterizerState = std::make_shared<DX11RasterizerState>();
	rasterizerState->InitRasterizerState(graphicEngine.m_device.Get(), graphicEngine.m_deviceContext.Get());
	model.Init(graphicEngine.m_device.Get(), doc, attrName);
}

void Renderer::Tick()
{
	context->SetVertexBuffer(model.m_mesh->m_vertexbuffer->m_vertexbuffer.Get());
	context->SetIndexBuffer(model.m_mesh->m_indexbuffer->m_IndexBuffer.Get());

	context->SetVertexShader();
	context->SetRasterizerState(rasterizerState->m_rasterizerstate.Get());
	context->SetSingleViewPort(&graphicEngine.m_viewport);
	context->SetSingleRenderTarget(graphicEngine.m_backBufferView.Get(),graphicEngine.m_DepthstencilView.Get());
	
	context->DrawIndexedInstanced()
}
