#pragma once
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include "Vertex.h"
class DirectX11VertexBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>  m_vertexbuffer;

	UINT m_sizeofElement = 0;
	UINT m_numElement = 0;
	

	

	HRESULT InitVertexBuffer(ID3D11Device* device, const std::vector<Vertex> v);

};

