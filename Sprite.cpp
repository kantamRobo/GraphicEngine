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
		//dds�t�@�C���̃p�X���w�肳��Ă�̂Ȃ�Adds�t�@�C������e�N�X�`�����쐬����B
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
		//�e�N�X�`�����w�肳��ĂȂ��B
		MessageBoxA(nullptr, "initData.m_ddsFilePath��m_textures�̂ǂ��炩�Ɏg�p����e�N�X�`���̏���ݒ肵�Ă��������B", "�G���[", MB_OK);
		std::abort();
	}
}

void InitShader(const SpriteInitData& initData)
{
	if (initData.m_fxFilePath == nullptr)
	{
		MessageBoxA(nullptr, "fx�t�@�C�����w�肳��Ă��܂���B", "�G���[", MB_OK);
		std::abort();
	}

	//�V�F�[�_�[�����[�h����B
	m_vs.LoadVS(initData.m_fxFilePath, initData.m_vsEntryPointFunc);
	m_ps.LoadPS(initData.m_fxFilePath, initData.m_psEntryPoinFunc);
}

void Sprite::InitDescriptorHeap(const SpriteInitData& initData)
{

	if (m_textureExternal[0] != nullptr) {
		//�O���̃e�N�X�`�����w�肳��Ă���B
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
		//�g���V�F�[�_�[���\�[�X�r���[�B
		m_descriptorHeap.RegistShaderResource(
			EXPAND_SRV_REG__START_NO,
			*initData.m_expandShaderResoruceView
		);
	}
	m_descriptorHeap.RegistConstantBuffer(0, m_constantBufferGPU);
	if (m_userExpandConstantBufferCPU != nullptr) {
		//���[�U�[�g���̒萔�o�b�t�@��b1�Ɋ֘A�t������B
		m_descriptorHeap.RegistConstantBuffer(1, m_userExpandConstantBufferGPU);
	}
	m_descriptorHeap.Commit();
}
