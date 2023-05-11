#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stdafx.h"
#include "RenderContext.h"
#include "Shader.h"
class Material
{
public:
	//assimp�̃}�e���A����񂩂珉����
	void InitFromAssimpMaterial(const aiMaterial* aiMat, int numSrv, int numCbv,
		UINT offsetInDescriptorsFromTableStartCB,
		UINT offsetInDescriptorsFromTableStartSRV,
		D3D12_FILTER samplerFilter);

	//�����_�����O���J�n����Ƃ��ɌĂяo���֐�

	void BeginRender(std::shared_ptr<RenderContext> rc, int hasSkin);

	std::shared_ptr<Texture> GetAlbedoMap()
	{

	}

	//�@���}�b�v���擾
	std::shared_ptr<Texture> GetNormalMap()
	{
		return
	}

	std::shared_ptr<Texture> GetSpecularMap()
	{
		return
	}

	//���˃}�b�v���擾

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
	/// �V�F�[�_�[�̏������B
	/// </summary>
	/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
	/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="vsEntryPointFunc">�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>

	void InitShaders(const char* fxFilePath,
		const char* vsEntrypointFunc,
		const char* vsSkinEntrypointFunc,
		const char* psEntryPointFunc);

	//�e�N�X�`����������

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

