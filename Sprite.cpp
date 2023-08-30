#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"
#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
