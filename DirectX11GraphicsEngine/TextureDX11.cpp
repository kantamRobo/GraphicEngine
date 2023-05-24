
#include "TextureDX11.h"
#include <DirectXTex.h>
#include "IShaderResource.h"


 TextureDX11::~TextureDX11()
{
}

 void TextureDX11::LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath)
 {
	 
	 DirectX::CreateDDSTextureFromFile(device, filePath,m_texture.GetAddressOf(), m_shaderResourceView.Get());

 }

 void TextureDX11::LoadTextureFromMemory(const char* memory, unsigned int size)
 {
	 
 }

 void TextureDX11::InitFromMemory(const char* memory, unsigned int size)
 {

 }

 void TextureDX11::InitFromD3DResource(ComPtr<ID3D11Texture2D> texture)
 {
 }

 void TextureDX11::RegistShaderResourceView(int bufferNo)
 {
	 
 }
