#pragma once
#define NOMINMAX
#include <DirectXTex.h>
#include "stdafx.h"

	class DX11Texture
	{
	public:
		DX11Texture(const wchar_t* filePath)
		{

		}
		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

		ComPtr<ID3D11Resource> m_textureresource;

		ComPtr<ID3D11Texture2D> m_texture;

		ComPtr<ID3D11SamplerState> m_samplerstate;

		D3D11_TEXTURE2D_DESC texdesc;

		D3D11_SAMPLER_DESC m_samplerDesc;
		
		
		
		~DX11Texture();
		
		
		/// <summary>
		/// テクスチャが有効か判定。
		/// </summary>
		/// <returns>trueが返ってきたら有効。</returns>
		bool IsValid() const
		{
			return m_texture != nullptr;
		}

		

	private:
		/// <summary>
		/// DDSファイルからテクスチャをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath);
		/// <summary>
		/// メモリからテクスチャをロード。
		/// </summary>
		/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス。</param>
		/// <param name="size">テクスチャのサイズ。</param>
		/// <param name="ge12">Dx12版のグラフィックスエンジン</param>
		/// <param name="device">D3Dデバイス</param>
		void LoadTextureFromMemory(const char* memory, unsigned int size);

		
	};

