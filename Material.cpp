#include "Material.h"
#include "Engine.h"
#include "NullTextureMaps.h"
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
extern Engine* g_engine;
extern GraphicsEngine* g_graphicsEngine;
void Material::BeginRender(std::shared_ptr<RenderContext> rc, int hasSkin)
{
	rc->SetRootSignature(m_rootSignature.Get().Get());
	if (hasSkin)
	{
		rc->SetPipelineState(m_transSkinModelPipelineState.Get().Get());
	}
	else
	{
		rc->SetPipelineState(m_transNonSkinModelPipelineState.Get().Get());
	}
}

void Material::InitPipelineState(const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat)
{
	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get().Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob().Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel->GetCompiledBlob().Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);


	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
#ifdef  TK_ENABLE_ALPHA_TO_COVERAGE
	psoDesc.BlendState.AlphaToCoverageEnable = TRUE;
#endif //  TK_ENABLE_ALPHA_TO_COVERAGE
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	int numRenderTarget = 0;
	for (auto& format : colorBufferFormat)
	{
		if (format == DXGI_FORMAT_UNKNOWN)
		{
			break;
		}
		psoDesc.RTVFormats[numRenderTarget] = colorBufferFormat[numRenderTarget];
		numRenderTarget++;
	}
	psoDesc.NumRenderTargets = numRenderTarget;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.InitGraphicPipelineState(psoDesc);

	//続いてスキン無しモデル用を作成
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob().Get());
	m_nonSkinModelPipelineState.InitGraphicPipelineState(psoDesc);

	//続いて半透明マテリアル用
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob().Get());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	m_transSkinModelPipelineState.InitGraphicPipelineState(psoDesc);

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob().Get());
	m_transNonSkinModelPipelineState.InitGraphicPipelineState(psoDesc);

}

void Material::InitShaders(
	const char* fxFilePath, 
	const char* vsEntrypointFunc, 
	const char* vsSkinEntrypointFunc, 
	const char* psEntryPointFunc)
{
	//スキン無しモデル用のシェーダーをロードする
	m_vsNonSkinModel = g_engine->GetShaderFromBank(fxFilePath, vsEntrypointFunc);
	if (m_vsNonSkinModel == nullptr)
	{
		m_vsNonSkinModel = std::make_shared<Shader>();
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntrypointFunc);
		g_engine->RegistShaderToBank(fxFilePath, vsEntrypointFunc, m_vsNonSkinModel.get());
	}
	//スキンありモデル用のシェーダーをロードする
	m_vsSkinModel = g_engine->GetShaderFromBank(fxFilePath, vsSkinEntrypointFunc);
	if (m_vsSkinModel == nullptr)
	{
		m_vsSkinModel = std::make_shared<Shader>();
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntrypointFunc);
		g_engine->RegistShaderToBank(fxFilePath, vsSkinEntrypointFunc, m_vsSkinModel.get());
	}

	m_psModel = g_engine->GetShaderFromBank(fxFilePath, psEntryPointFunc);

	if (m_psModel == nullptr)
	{
		m_psModel = std::make_shared<Shader>();
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		g_engine->RegistShaderToBank(fxFilePath, psEntryPointFunc, m_psModel.get());
	}

}


void Material::InitFromAssimpMaterial(
	const aiMaterial* aiMat, 
	int numSrv, int numCbv, 
	const std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
	UINT offsetInDescriptorsFromTableStartCB, 
	UINT offsetInDescriptorsFromTableStartSRV, 
	D3D12_FILTER samplerFilter,
	const char* fxfilePath, 
	const char* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc)
{
	InitTexture(aiMat);

	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap->IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap->IsValid() ? 1 : 0;
	m_constantBuffer.InitConstBuffer(sizeof(SMaterialParam), &matParam);

	D3D12_STATIC_SAMPLER_DESC samplerDescArray[2];

	samplerDescArray[0].Filter = samplerFilter;
	samplerDescArray[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescArray[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescArray[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDescArray[0].MipLODBias = 0;
	samplerDescArray[0].MaxAnisotropy = 0;
	samplerDescArray[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDescArray[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDescArray[0].MinLOD = 0.0f;
	samplerDescArray[0].MaxLOD = D3D12_FLOAT32_MAX;
	samplerDescArray[0].ShaderRegister = 0;
	samplerDescArray[0].RegisterSpace = 0;
	samplerDescArray[0].ShaderRegister = 0;
	samplerDescArray[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//シャドウマップ用のサンプラ。
	samplerDescArray[1] = samplerDescArray[0];
	//比較対象の値が小さければ０、大きければ１を返す比較関数を設定する。
	samplerDescArray[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDescArray[1].ComparisonFunc = D3D12_COMPARISON_FUNC_GREATER;
	samplerDescArray[1].MaxAnisotropy = 1;
	samplerDescArray[1].ShaderRegister = 1;

	m_rootSignature.InitRootSignature(samplerDescArray, 2, numCbv, numSrv, 8, offsetInDescriptorsFromTableStartCB, offsetInDescriptorsFromTableStartSRV);

	if (fxfilePath != nullptr && strlen(fxfilePath) > 0)
	{
		InitShaders(fxfilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);

		InitPipelineState(colorBufferFormat);
	}

}



void Material::InitTexture(const aiMaterial* aiMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;

	//アルベドマップ
	{
		
		if (aiMat != nullptr)
		{
			
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, filePath);
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, map);
			
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, mapSize);

		}
		else
		{
			filePath = nullTextureMaps->GetAlbedoMapFilePath();
			map = nullTextureMaps->GetAlbedoMap().get();
			mapSize = nullTextureMaps->GetAlbedoMapSize();
		}

		auto albedoMap = g_engine->GetTextureFromBank(filePath);

		if (albedoMap == nullptr)
		{
			albedoMap = std::make_shared<Texture>();
			albedoMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, albedoMap.get());
		}
		m_albedoMap = albedoMap;
	}

	//法線マップ
	{
		if (aiMat != nullptr)
		{
			//TODO
			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, filePath);
			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, map);

			aiMat->Get(AI_MATKEY_TEXTURE_NORMALS, mapSize);
		}
		auto normalMap = g_engine->GetTextureFromBank(filePath);
		if (normalMap == nullptr)
		{
			normalMap = std::make_shared<Texture>();
			normalMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, normalMap.get());
		}
		m_normalMap = normalMap;

	}



	//反射マップ
	{
		if (aiMat != nullptr)
		{

			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, filePath);
			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, map);

			aiMat->Get(AI_MATKEY_TEXTURE_REFLECTION, mapSize);
		}
		auto specularmap = g_engine->GetTextureFromBank(filePath);
		if (specularmap == nullptr)
		{
			specularmap = std::make_shared<Texture>();
			specularmap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, specularmap.get());
		}
	 m_specularMap = specularmap;

	}

	
}
