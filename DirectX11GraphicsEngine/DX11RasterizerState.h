#pragma once
#include "DirectX11GraphicEngine.h"
#include <d3d11.h>
#include <wrl.h>
class DX11RasterizerState
{
public:
	Microsoft::WRL::ComPtr< ID3D11RasterizerState> m_rasterizerstate;
	D3D11_RASTERIZER_DESC rsdesc = {};
	
	void InitRasterizerState(const DirectX11GraphicEngine* p_engine, ID3D11DeviceContext* p_context, D3D11_VIEWPORT* viewport);
};

