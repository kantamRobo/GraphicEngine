#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl.h>
class DirectX11IndexBuffer
{
public:
	Microsoft::WRL::ComPtr< ID3D11Buffer> m_IndexBuffer = nullptr;
	UINT m_sizeofElement = 0;
	UINT m_numElement = 0;
	
	HRESULT InitIndexbuffer(ID3D11Device* device, std::vector<UINT> indices);
};

