#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "RenderContext.h"
#include "Shader.h"
class Material
{
public:
	//assimpのマテリアル情報から初期化
	void InitFromAssimpMaterial(const aiMaterial* aiMat, int numSrv, int numCbv,
		UINT offsetInDescriptorsFromTableStartCB,
		UINT offsetInDescriptorsFromTableStartSRV,
		D3D12_FILTER samplerFilter,
		const char* fxfilePath);

	//レンダリングを開始するときに呼び出す関数

	void BeginRender(std::shared_ptr<RenderContext> rc, int hasSkin);

	std::shared_ptr<Texture> GetAlbedoMap()
	{
		return m_albedoMap;
	}

	//法線マップを取得
	std::shared_ptr<Texture> GetNormalMap()
	{
		return m_normalMap;
	}

	std::shared_ptr<Texture> GetSpecularMap()
	{
		return m_specularMap;
	}

	//反射マップを取得

	std::shared_ptr<Texture> GetReflectionMap()
	{
		return m_reflectionMap;
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

	
	void InitFromAssimpMaterial(const aiMaterial* aiMat, int numSrv, int numCbv, const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat, UINT offsetInDescriptorsFromTableStartCB, UINT offsetInDescriptorsFromTableStartSRV, D3D12_FILTER samplerFilter, const char* fxfilePath, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc);

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

	
	ConstantBuffer m_constantBuffer;				//定数バッファ。
	RootSignature m_rootSignature;					//ルートシグネチャ。
	PipelineState m_nonSkinModelPipelineState;		//スキンなしモデル用のパイプラインステート。
	PipelineState m_skinModelPipelineState;			//スキンありモデル用のパイプラインステート。
	PipelineState m_transSkinModelPipelineState;	//スキンありモデル用のパイプラインステート(半透明マテリアル)。
	PipelineState m_transNonSkinModelPipelineState;	//スキンなしモデル用のパイプラインステート(半透明マテリアル)。

	std::shared_ptr<Shader> m_vsNonSkinModel = nullptr;
	std::shared_ptr<Shader> m_vs_SkinModel = nullptr;
	std::shared_ptr<Shader> m_psModel = nullptr;
	std::shared_ptr<Shader> m_vsSkinModel = nullptr;
	std::shared_ptr<Shader> m_psModel = nullptr;


};

