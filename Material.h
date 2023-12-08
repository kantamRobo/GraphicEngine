#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "RenderContext.h"
#include "Shader.h"
class Material
{
public:
	//assimp�̃}�e���A����񂩂珉����
	void InitFromAssimpMaterial(const aiMaterial* aiMat, int numSrv, int numCbv,
		UINT offsetInDescriptorsFromTableStartCB,
		UINT offsetInDescriptorsFromTableStartSRV,
		D3D12_FILTER samplerFilter,
		const char* fxfilePath);

	//�����_�����O���J�n����Ƃ��ɌĂяo���֐�

	void BeginRender(std::shared_ptr<RenderContext> rc, int hasSkin);

	std::shared_ptr<Texture> GetAlbedoMap()
	{
		return m_albedoMap;
	}

	//�@���}�b�v���擾
	std::shared_ptr<Texture> GetNormalMap()
	{
		return m_normalMap;
	}

	std::shared_ptr<Texture> GetSpecularMap()
	{
		return m_specularMap;
	}

	//���˃}�b�v���擾

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

	
	ConstantBuffer m_constantBuffer;				//�萔�o�b�t�@�B
	RootSignature m_rootSignature;					//���[�g�V�O�l�`���B
	PipelineState m_nonSkinModelPipelineState;		//�X�L���Ȃ����f���p�̃p�C�v���C���X�e�[�g�B
	PipelineState m_skinModelPipelineState;			//�X�L�����胂�f���p�̃p�C�v���C���X�e�[�g�B
	PipelineState m_transSkinModelPipelineState;	//�X�L�����胂�f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B
	PipelineState m_transNonSkinModelPipelineState;	//�X�L���Ȃ����f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)�B

	std::shared_ptr<Shader> m_vsNonSkinModel = nullptr;
	std::shared_ptr<Shader> m_vs_SkinModel = nullptr;
	std::shared_ptr<Shader> m_psModel = nullptr;
	std::shared_ptr<Shader> m_vsSkinModel = nullptr;
	std::shared_ptr<Shader> m_psModel = nullptr;


};

