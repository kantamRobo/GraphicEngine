#pragma once
#include <d3d11.h>
#include <vector>
#include "Vertex.h"
class DirectX11VertexBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>  m_vertexbuffer;

	
	HRESULT Init(const std::vector<Vertex> v);
};

