
#include "TextureDX11.h"
#include <DirectXTex.h>
#include "IShaderResource.h"


 TextureDX11::~TextureDX11()
{
}

 void TextureDX11::LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath)
 {
	 
	 DirectX::CreateDDSTextureFromFile(device, filePath,m_texture.GetAddressOf(),m_shaderResourceView.ReleaseAndGetAddressOf());

 }

 void TextureDX11::LoadTextureFromMemory(const char* memory, unsigned int size)
 {
	 
 }

