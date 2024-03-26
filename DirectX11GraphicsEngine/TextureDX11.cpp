
#include "DX11Texture.h"
#include "IShaderResource.h"
#include <DDSTextureLoader.h>

a

 DX11Texture::~DX11Texture()
{
}

 void DX11Texture::LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath)
 {
     DirectX::ScratchImage image;
	auto hr = LoadFromDDSFile(filePath, DirectX::DDS_FLAGS_NONE, nullptr, image);
	
	// �ٕ����t�B���^�����O��ԁAWrap���[�h
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���`�t�B���^�����O
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 0;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	// �X�e�[�g�I�u�W�F�N�g�쐬
	
	device->CreateSamplerState(&desc, &m_samplerstate);
	DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_shaderResourceView.GetAddressOf());
 }

 //https://learn.microsoft.com/ja-jp/windows/uwp/gaming/applying-textures-to-primitives

 void DX11Texture::LoadTextureFromMemory(const char* memory, unsigned int size)
 {
	 
 }

