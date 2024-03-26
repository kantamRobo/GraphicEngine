#include "DX11RasterizerState.h"




void DX11RasterizerState::InitRasterizerState(const DirectX11GraphicEngine* p_engine,ID3D11DeviceContext* p_context,D3D11_VIEWPORT* viewport)
{
	//‚½‚½‚«‘ä
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.ScissorEnable = true;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = false;
	desc.DepthBias = 0;
	
	
	auto hr = p_engine->m_device->CreateRasterizerState(&desc, m_rasterizerstate.GetAddressOf());
	UINT count = 1;
	p_context->RSGetViewports(&count, viewport);
}

