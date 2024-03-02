#pragma once
#include <d3d11.h>
#include <wrl.h>
class DX11RasterizerState
{
public:
	Microsoft::WRL::ComPtr< ID3D11RasterizerState> m_rasterizerstate;
	
	void InitRasterizerState(ID3D11Device* p_device, ID3D11DeviceContext* p_context);
};

