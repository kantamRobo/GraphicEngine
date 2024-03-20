#pragma once
#include <d3d11.h>
#include <memory>
#include "DirectX11VertexBuffer.h"
#include "DirectX11IndexBuffer.h"
struct DX11Mesh
{
	std::shared_ptr<DirectX11VertexBuffer> m_vertexbuffer;
	std::shared_ptr <DirectX11IndexBuffer> m_indexbuffer;

	DX11Mesh(){
		m_vertexbuffer = std::make_shared<DirectX11VertexBuffer>();
		m_indexbuffer = std::make_shared<DirectX11IndexBuffer>();
	}
};