#pragma once
#include "stdafx.h"
class Shader
{
public:
	/// <summary>
	/// ピクセルシェーダーをロード。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	/// <param name="entryFuncName">エントリーポイントの関数名。</param>
	void LoadPS(const char* filePath, const char* entryFuncName);
	/// <summary>
	/// 頂点シェーダーをロード。
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="entryFuncName">エントリーポイントの関数名。</param>
	void LoadVS(const char* filePath, const char* entryFuncName);
	/// <summary>
	/// コンピュートシェーダーをロード。
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
	/// 初期化済み？
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

