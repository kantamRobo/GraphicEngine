/// <summary>
/// ���b�V���p�[�c�N���X�B
/// </summary>

#pragma once
class ConstantBuffer;
class RenderContext;
class Skeleton;
class SMaterial;
class IndexBuffer;
class VertexBuffer;
class IShaderResource;

class DescriptorHeap;
#include "StructuredBuffer.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "Skeleton.h"
#include "Matrix.h"


const int MAX_MODEL_EXPAND_SRV = 6;	//�g��SRV�̍ő吔�B

/// <summary>
/// ���b�V��
/// </summary>
struct SMesh {
	VertexBuffer m_vertexBuffer;						//���_�o�b�t�@�B
	std::vector< std::shared_ptr<IndexBuffer> >		m_indexBufferArray;	//�C���f�b�N�X�o�b�t�@�B
	std::vector < std::shared_ptr< SMaterial> > m_materials;			//�}�e���A���B
	std::vector<int>				skinFlags;				//�X�L���������Ă��邩�ǂ����̃t���O�B
};

/// <summary>
/// ���b�V���p�[�c�B
/// </summary>
class MeshParts {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~MeshParts();
	void InitFromGltfFile(const aiScene* scene, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, void* expandData, int expandDataSize, const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter);
	
	
	void DrawCommon(std::shared_ptr<RenderContext> rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="mView">�r���[�s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	/// <param name="light">���C�g</param>
	
	void Draw(std::shared_ptr<RenderContext> rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);

	
	void DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	/// <summary>
	/// �X�P���g�����֘A�t����B
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	void BindSkeleton(std::shared_ptr<Skeleton> skeleton);
	/// <summary>
	/// ���b�V���ɑ΂��Ė₢���킹���s���B
	/// </summary>
	/// <param name="queryFunc">�N�G���֐�</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		for (const auto& mesh : m_meshs) {
			queryFunc(*mesh);
		}
	}
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���쐬�B
	/// </summary>
	void CreateDescriptorHeaps();
	void CreateMeshFromAssimpMesh(const Microsoft::glTF::Document& doc, int meshNo, int& materialNum, const char* fxFilePath, const char* vsEntryPointFunc, const char* vsSkinEntryPointFunc, const char* psEntryPointFunc, const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat, D3D12_FILTER samplerFilter);
private:
	/// <summary>
	/// tkm���b�V�����烁�b�V�����쐬�B
	/// </summary>
	/// <param name="mesh">���b�V��</param>
	/// <param name="meshNo">���b�V���ԍ�</param>
	/// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
	/// <param name="vsEntryPointFunc">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="vsSkinEntryPointFunc">�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="psEntryPointFunc">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="colorBufferFormat">���̃��f���������_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g</param>
	/// <param name="samplerFilter">�T���v���t�B���^</param>
	void CreateMeshFromAssimpMesh(
		
		int meshNo,
		int& materialNum,
		const char* fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		D3D12_FILTER samplerFilter
	);
	/// <summary>
	/// �`�揈���̋��ʏ���
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	/// <param name="mWorld">���[���h�s��</param>
	/// <param name="mView">�r���[�s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	void DrawCommon(RenderContext& rc, const EngineMath::Matrix& mWorld, const EngineMath::Matrix& mView, const EngineMath::Matrix& mProj);
	
private:
	//�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ��B
	const int EXPAND_SRV_REG__START_NO = 10;
	//�P�̃}�e���A���Ŏg�p�����SRV�̐��B
	const int NUM_SRV_ONE_MATERIAL = EXPAND_SRV_REG__START_NO + MAX_MODEL_EXPAND_SRV;
	//�P�̃}�e���A���Ŏg�p�����CBV�̐��B
	const int NUM_CBV_ONE_MATERIAL = 2;
	/// <summary>
	/// �萔�o�b�t�@�B
	/// </summary>
	/// <remarks>
	/// ���̍\���̂�ύX������ASimpleModel.fx��CB���ύX����悤�ɁB
	/// </remarks>
	struct SConstantBuffer {
		EngineMath::Matrix mWorld;		//���[���h�s��B
		EngineMath::Matrix mView;		//�r���[�s��B
		EngineMath::Matrix mProj;		//�v���W�F�N�V�����s��B
	};
	ConstantBuffer m_commonConstantBuffer;					//���b�V�����ʂ̒萔�o�b�t�@�B
	ConstantBuffer m_expandConstantBuffer;					//���[�U�[�g���p�̒萔�o�b�t�@
	std::array<std::shared_ptr<IShaderResource>, MAX_MODEL_EXPAND_SRV> m_expandShaderResourceView = { nullptr };	//���[�U�[�g���V�F�[�_�[���\�[�X�r���[�B
	std::shared_ptr<StructuredBuffer> m_boneMatricesStructureBuffer;	//�{�[���s��̍\�����o�b�t�@�B
	std::vector< std::shared_ptr<SMesh> > m_meshs;						//���b�V���B
	//std::vector< DescriptorHeap > m_descriptorHeap;	//�f�B�X�N���v�^�q�[�v�B
	std::shared_ptr<DescriptorHeap> m_descriptorHeap;					//�f�B�X�N���v�^�q�[�v�B
	std::shared_ptr<Skeleton> m_skeleton = nullptr;						//�X�P���g���B
	void* m_expandData = nullptr;						//���[�U�[�g���f�[�^�B
};