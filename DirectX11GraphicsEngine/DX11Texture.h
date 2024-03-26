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
		/// �e�N�X�`�����L��������B
		/// </summary>
		/// <returns>true���Ԃ��Ă�����L���B</returns>
		bool IsValid() const
		{
			return m_texture != nullptr;
		}

		

	private:
		/// <summary>
		/// DDS�t�@�C������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void LoadTextureFromDDSFile(ID3D11Device* device, const wchar_t* filePath);
		/// <summary>
		/// ����������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X�B</param>
		/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
		/// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
		/// <param name="device">D3D�f�o�C�X</param>
		void LoadTextureFromMemory(const char* memory, unsigned int size);

		
	};

