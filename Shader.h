#pragma once
#include "stdafx.h"
class Shader
{
public:
	/// <summary>
	/// �s�N�Z���V�F�[�_�[�����[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
	void LoadPS(const char* filePath, const char* entryFuncName);
	/// <summary>
	/// ���_�V�F�[�_�[�����[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
	void LoadVS(const char* filePath, const char* entryFuncName);
	/// <summary>
	/// �R���s���[�g�V�F�[�_�[�����[�h�B
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="entryFuncName"></param>
	void LoadCS(const char* filePath, const char* entryFuncName);

	ComPtr<ID3DBlob> GetCompiledBlob()const
	{
		return m_blob;
	}
	ComPtr<IDxcBlob> GetCompiledDxcBlob()
	{
		return m_dxcBlob;
	}
	/// <summary>
	/// �������ς݁H
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}
private:
	void LoadShader(const char* filePath, const char* entryFuncName, const char* shaderModel);
private:
	ComPtr<ID3DBlob> m_blob = nullptr;
	ComPtr <IDxcBlob> m_dxcBlob = nullptr;
	bool m_isInited = false;
};

