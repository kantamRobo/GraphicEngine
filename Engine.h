#pragma once

#include "TResourceBank.h"

class Engine
{

private:
	
	TResourceBank<Shader> m_shaderBank;
	TResourceBank<Texture> m_textureBank;
public:
	//デストラクタ
	~Engine();

	void BeginFrame();

	void EndFrame();

	void InitEngine(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);


	//バンクからテクスチャを取得
	std::shared_ptr<Texture> GetTextureFromBank(const char* filePath)
	{
		return m_textureBank.Get(filePath);
	}


	/// <summary>
	/// テクスチャをバンクに登録。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <param name="texture">テクスチャ。</param>
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