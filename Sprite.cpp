#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "Texture.h"
#include "RenderContext.h"
#include "Shader.h"
#include "Sprite.h"


namespace {
	
	struct SSimpleVertex {
		EngineMath::Vector4 pos;
		EngineMath::Vector2 tex;

	};

	

}

const EngineMath::Vector2 Sprite::DEFAULT_PIVOT = { 0.5f,0.5f };

Sprite::~Sprite()
{

}

void Sprite::InitTextures(const SpriteInitData& initData)
{
	if (initData.m_ddsFilePath[0] != nullptr) {
		//ddsファイルのパスが指定されてるのなら、ddsファイルからテクスチャを作成する。
		int texNo = 0;
		while (initData.m_ddsFilePath[texNo] && texNo < MAX_TEXTURE)
		{
			wchar_t wddsFfilePath[1024];
			mbstowcs(wddsFfilePath, initData.m_ddsFilePath[texNo], 1023);
			m_textures[texNo].InitFromDDSFile(wddsFilePath);
			texNo++;
		}
		m_numTexture = texNo;
	}
	else if (initData.m_textures[0] != nullptr)
	{
		int texNo = 0;

		while (initData.m_textures[texNo] != nullptr)
		{
			int texNo = 0;
			while (initData.m_textures[texNo] != nullptr)
			{
				m_textureExternal[texNo] = initData.m_textures[texNo];
				texNo++;
			}
			m_numTexture = texNo;
		}
	}
	else {
		//テクスチャが指定されてない。
		MessageBoxA(nullptr, "initData.m_ddsFilePathかm_texturesのどちらかに使用するテクスチャの情報を設定してください。", "エラー", MB_OK);
		std::abort();
	}
}

void InitShader(const SpriteInitData& initData)
{
	if (initData.m_fxFilePath == nullptr)
	{
		MessageBoxA(nullptr, "fxファイルが指定されていません。", "エラー", MB_OK);
		std::abort();
	}

	//シェーダーをロードする。
	m_vs.LoadVS(initData.m_fxFilePath, initData.m_vsEntryPointFunc);
	m_ps.LoadPS(initData.m_fxFilePath, initData.m_psEntryPoinFunc);
}

void Sprite::InitDescriptorHeap(const SpriteInitData& initData)
{

	if (m_textureExternal[0] != nullptr) {
		//外部のテクスチャが指定されている。
		for (int texNo = 0; texNo < m_numTexture; texNo++) {
			m_descriptorHeap.RegistShaderResource(texNo, *m_textureExternal[texNo]);
		}
	}
	else {
		for (int texNo = 0; texNo < m_numTexture; texNo++) {
			m_descriptorHeap.RegistShaderResource(texNo, m_textures[texNo]);
		}
	}
	if (initData.m_expandShaderResoruceView != nullptr) {
		//拡張シェーダーリソースビュー。
		m_descriptorHeap.RegistShaderResource(
			EXPAND_SRV_REG__START_NO,
			*initData.m_expandShaderResoruceView
		);
	}
	m_descriptorHeap.RegistConstantBuffer(0, m_constantBufferGPU);
	if (m_userExpandConstantBufferCPU != nullptr) {
		//ユーザー拡張の定数バッファはb1に関連付けする。
		m_descriptorHeap.RegistConstantBuffer(1, m_userExpandConstantBufferGPU);
	}
	m_descriptorHeap.Commit();
}
void Sprite::InitVertexBufferAndIndexBuffer(const SpriteInitData& initData)
{
	float halfW = m_size.x * 0.5f;
	float halfH = m_size.y * 0.5f;
	//頂点バッファのソースデータ
	SSimpleVertex verices[] =
	{
		{
			EngineMath::Vector4(-halfW,-halfH,0.0f,1.0f),
			EngineMath::Vector2(0.0f,1.0f),
		},
		{
			EngineMath::Vector4(halfW,-halfH,0.0f,1.0f),
			EngineMath::Vector2(0.0f,1.0f),

		},
		{
			EngineMath::Vector4(halfW,halfW,0.0f,1.0f),
			EngineMath::Vector2(1.0f,0.0f)

			}
		
	};

	unsigned short indices[] = { 0,1,2,3 };

	m_vertexBuffer.InitVertexBuffer(sizeof(vertices), sizeof(vertices[0]));
	m_vertexBuffer.Copy(vertices);

	m_indexBuffer.InitIndexBuffer(sizeof(indices), sizeof(indices[0]));
	m_indexBuffer.Copy(indices);
   }

void Sprite::InitPipelineState(const SpriteInitData& initData)
{
	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//パイプラインステートを作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	if (initData.m_alphaBlendMode == AlphaBlendMode_Trans)
	{
		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	}
	if (initData.m_alphaBlendMode == AlphaBlendMode_Add)
	{
		//加算合成。
		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	}

	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	for (auto& format : initData.m_colorBufferFormat) {
		if (format == DXGI_FORMAT_UNKNOWN) {
			break;
		}
		psoDesc.RTVFormats[psoDesc.NumRenderTargets] = format;
		psoDesc.NumRenderTargets++;
	}

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_pipelineState.Init(psoDesc);
}