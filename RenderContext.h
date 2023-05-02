#pragma once
#include "stdafx.h"

class ConstantBuffer;
class Texture;
class DescriptorHeap;
class RenderTarget;
class RenderContext
{
public:
	void InitRenderingContext(ComPtr<ID3D12GraphicsCommandList4> commandList)
	{
		m_commandList = commandList;
	}
private:
	enum { MAX_DESCRIPTOR_HEAP = 4 };
	enum { MAX_CONSTANT_BUFFER = 8 };
	enum { MAX_SHADER_RESOURCE = 16 };
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	std::array<ComPtr<ID3D12DescriptorHeap>, MAX_DESCRIPTOR_HEAP> m_descriptorHeaps;
	std::shared_ptr<ConstantBuffer>m_constantBuffers[MAX_CONSTANT_BUFFER];
	std::shared_ptr<Texture> m_shaderResources[MAX_SHADER_RESOURCE];

};

