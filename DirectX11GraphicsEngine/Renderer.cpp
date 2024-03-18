#include "Renderer.h"

void Renderer::Init(HWND hwnd, const Microsoft::glTF::Document& doc, const std::string& attrName)
{
	graphicEngine.CreateDevice();
	graphicEngine.CreateSwapChain(hwnd);
	graphicEngine.CreateRTV();
	context = std::make_shared<DX11RenderContext>(graphicEngine.m_deviceContext);
	model.Init(graphicEngine.m_device.Get(), doc, attrName);
}

void Renderer::Tick()
{
	context->SetVertexBuffer();
	context->SetIndexBuffer();

	context->SetVertexShader();
	context->SetRasterizerState();
	context->SetSingleViewPort();
	context->SetSingleRenderTarget();
	
	context->DrawIndexedInstanced()
}
