#pragma once
#include <d3d11.h>
#include <memory>
#include "DirectX11VertexBuffer.h"
#include "DirectX11IndexBuffer.h"
struct DX11Mesh
{
	std::shared_ptr<DirectX11VertexBuffer> m_vertexbuffer;
	std::shared_ptr <DirectX11IndexBuffer> m_indexbuffer;
};