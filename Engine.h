#pragma once

#include "TResourceBank.h"

class Engine
{

private:
	
	TResourceBank<Shader> m_shaderBank;
	TResourceBank<Texture> m_textureBank;
public:
	//�f�X�g���N�^
	~Engine();

	void BeginFrame();

	void EndFrame();

	void InitEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);


	//�o���N����e�N�X�`�����擾
	std::shared_ptr<Texture> GetTextureFromBank(const char* filePath)
	{
		return m_textureBank.Get(filePath);
	}


	/// <summary>
	/// �e�N�X�`�����o���N�ɓo�^�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	/// <param name="texture">�e�N�X�`���B</param>
	void RegistTextureToBank(const char* filePath, Texture* texture)
	{
		m_textureBank.Regist(filePath, texture);
	}

	std::shared_ptr<Shader> GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}

};

extern Engine* g_engine = nullptr;