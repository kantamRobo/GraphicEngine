#pragma once
#include "IShaderResource.h"
#include "stdafx.h"
class Texture:public IShaderResource
{
public:
	Texture(const wchar_t* filePath)
	{
		InitFromDDSFile(filePath);
	}
	~Texture();
	/// <summary>
		/// �t�@�C������e�N�X�`�������[�h����R���X�g���N�^
		/// </summary>
		/// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
	explicit Texture(const wchar_t* filePath);

	/// <summary>
	/// DDS�t�@�C������e�N�X�`��������������B
	/// </summary>
	/// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
	void InitFromDDSFile(const wchar_t* filePath);
	/// <summary>
	/// ����������e�N�X�`��������������B
	/// </summary>
	/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X</param>
	/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
	void InitFromMemory(const char* memory, unsigned int size);
	void InitFromD3DResource(ComPtr<ID3D12Resource> texture);
	/// <summary>
	/// D3D���\�[�X����e�N�X�`��������������B
	/// </summary>
	/// <param name="resrouce">D3D���\�[�X�B</param>
	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descriptorHandle"></param>
	void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

	/// <summary>
	/// �e�N�X�`�����L��������B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����L���B</returns>
	bool IsValid() const
	{
		return m_texture != nullptr;
	}

	bool IsValid()const
	{
		return m_texture != nullptr;
	}

	ComPtr<ID3D12Resource> Get()
	{
		return m_texture;
	}

private:
	/// <summary>
	/// DDS�t�@�C������e�N�X�`�������[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void LoadTextureFromDDSFile(const wchar_t* filePath);
	/// <summary>
	/// ����������e�N�X�`�������[�h�B
	/// </summary>
	/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X�B</param>
	/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
	/// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
	/// <param name="device">D3D�f�o�C�X</param>
	void LoadTextureFromMemory(const char* memory, unsigned int size);

private:
	ComPtr<ID3D12Resource> m_texture = nullptr;
	D3D12_RESOURCE_DESC m_textureDesc;
};

