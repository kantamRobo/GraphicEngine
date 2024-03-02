#include "DX11RasterizerState.h"




void DX11RasterizerState::InitRasterizerState(ID3D11Device* p_device,ID3D11DeviceContext* p_context)
{
	‚½‚½‚«‘ä
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	desc.ScissorEnable = false;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = true;
	
	auto hr = p_device->CreateRasterizerState(&desc, this->m_rasterizerstate.GetAddressOf());
	UINT count = 1;
	p_context->RSGetViewports(&count, &this->mDefaultViewport);
}

