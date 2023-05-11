#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stdafx.h"
#include "RenderContext.h"
#include "Shader.h"
class Material
{
public:
	//assimpのマテリアル情報から初期化
	void InitFromAssimpMaterial(const aiMaterial* aiMat, int numSrv, int numCbv,
		UINT offsetInDescriptorsFromTableStartCB,
		UINT offsetInDescriptorsFromTableStartSRV,
		D3D12_FILTER samplerFilter);

	//レンダリングを開始するときに呼び出す関数

	void BeginRender(std::shared_ptr<RenderContext> rc, int hasSkin);

	std::shared_ptr<Texture> GetAlbedoMap()
	{

	}

	//法線マップを取得
	std::shared_ptr<Texture> GetNormalMap()
	{
		return
	}

	std::shared_ptr<Texture> GetSpecularMap()
	{
		return
	}

	//反射マップを取得

	std::shared_ptr<Texture> GetReflectionMap()
	{
		return 
	}

	std::shared_ptr<ConstantBuffer> GetConstantBuffer()
	{
		return 
	}

private:

	void InitPipelineState(const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat);

	/// <summary>
	/// シェーダーの初期化。
	/// </summary>
	/// <param name="fxFilePath">fxファイルのファイルパス</param>
	/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="vsEntryPointFunc">スキンありマテリアル用の頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>

	void InitShaders(const char* fxFilePath,
		const char* vsEntrypointFunc,
		const char* vsSkinEntrypointFunc,
		const char* psEntryPointFunc);

	//テクスチャを初期化

	void InitTexture(const aiMaterial* aiMat);

private:

	struct SMaterialParam {
		int hasNormalMap;
		int hasSpecMap;

	};
	std::shared_ptr<Texture> m_albedoMap;
	std::shared_ptr<Texture> m_normalMap;
	std::shared_ptr<Texture> m_specularMap;
	std::shared_ptr<Texture> m_reflectionMap;
	std::shared_ptr<Texture> m_refractionMap;
	std::shared_ptr<Shader> m_vsNonSkingModel = nullptr;
	std::shared_ptr<Shader> m_vsSkinModel = nullptr;
	std::shared_ptr<Shader> m_psModel = nullptr;
};

