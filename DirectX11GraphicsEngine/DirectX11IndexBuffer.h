#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl.h>
class DirectX11IndexBuffer
{
public:
	Microsoft::WRL::ComPtr< ID3D11Buffer> m_IndexBuffer = nullptr;

	HRESULT InitIndexbuffer(std::vector<UINT> indices);
};

