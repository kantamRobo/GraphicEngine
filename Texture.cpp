#include "IShaderResource.h"
#include <ResourceUploadBatch.h>

#include "GraphicsEngine.h"

#include "stdafx.h"
#include "Texture.h"

extern GraphicsEngine* g_graphicsEngine = nullptr;
Texture::~Texture()
{
	if (m_texture) {
		m_texture->Release();
	}
}

Texture::Texture(const wchar_t* filePath)
{
	InitFromDDSFile(filePath);
}

void Texture::InitFromDDSFile(const wchar_t* filePath)
{

	LoadTextureFromDDSFile(filePath);
}

void Texture::InitFromMemory(const char* memory, unsigned int size)
{
	LoadTextureFromMemory(memory, size);

}

void Texture::InitFromD3DResource(ID3D12Resource* texture)
{
	if (m_texture)
	{
		m_texture->Release();
	}
	m_texture = texture;
	m_texture->AddRef();
	m_textureDesc = m_texture->GetDesc();
}

void Texture::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
	if (m_texture)
	{
		auto device = g_graphicsEngine->GetD3DDevice();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = m_textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_textureDesc.MipLevels;
		device->CreateShaderResourceView(m_texture.Get(), &srvDesc, descriptorHandle);
	}
}

void Texture::LoadTextureFromDDSFile(const wchar_t* filePath)
{
	auto device = g_graphicsEngine->GetD3DDevice();
	DirectX::ResourceUploadBatch re(device.Get());
	re.Begin();
	ComPtr<ID3D12Resource> texture;
	auto hr = DirectX::CreateDDSTextureFromFileEx(
		device.Get(),
		re,
		filePath,
		0,
		D3D12_RESOURCE_FLAG_NONE,
		DirectX::DDS_LOADER_DEFAULT,
		&texture
	);
	re.End(g_graphicsEngine->GetCommandQueue().Get());

	if (FAILED(hr)) {
		//テクスチャの作成に失敗しました。
		return;
	}

	m_texture = texture;
	m_textureDesc = m_texture->GetDesc();
}

void Texture::LoadTextureFromMemory(const char* memory, unsigned int size)
{
	auto device = g_graphicsEngine->GetD3DDevice();
	DirectX::ResourceUploadBatch re(device.Get());
	re.Begin();
	ID3D12Resource* texture;
	auto hr = DirectX::CreateDDSTextureFromMemoryEx(
		device.Get(),
		re,
		(const uint8_t*)memory,
		size,
		0,
		D3D12_RESOURCE_FLAG_NONE,
		DirectX::DDS_LOADER_DEFAULT,
		&texture
	);
	re.End(g_graphicsEngine->GetCommandQueue().Get());

	if (FAILED(hr)) {
		//テクスチャの作成に失敗しました。
		return;
	}

	m_texture = texture;
	m_textureDesc = m_texture->GetDesc();
}
