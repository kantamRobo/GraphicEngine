#include "StructuredBuffer.h"
#include <string>
#include "Matrix.h"
class RenderContext;
class Skeleton;
class Material;
class IShaderResource;

const int MAX_MODEL_EXPAND_SRV = 6;	//�g��SRV�̍ő吔�B
const int  MAX_RENDERING_TARGET = 8;
struct SMesh {
	VertexBuffer m_vertexbuffer;
	std::vector< std::shared_ptr<IndexBuffer> >		m_indexBufferArray;
	std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<int>				skinFlags;
};

class MeshParts {
public:
	~MeshParts();

	void InitFromAssimpFile(

		const  std::string& fxFilePath,
		const char* vsEntryPointFunc,
		const char* vsSkinEntryPointFunc,
		const char* psEntryPointFunc,
		void* expandData,
		int expandDataSize,
		const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView,
		const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat,
		D3D12_FILTER samplerFilter

	);

	void Draw(std::shared_ptr<RenderContext>rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);


	void DrawInstancing(std::shared_ptr<RenderContext>rc,int numInstance, const Matrix& mView, const Matrix& mProj);


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

	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		for (int i = 0; i < m_meshs.size(); i++) {
			queryFunc(*m_meshs[i], m_descriptorHeap);
		}
	}

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���쐬�B
	/// </summary>
	void CreateDescriptorHeaps();


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
	void CreateMeshFromTkmMesh(
		const TkmFile::SMesh& mesh,
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
	void DrawCommon(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj);

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
		Matrix mWorld;		//���[���h�s��B
		Matrix mView;		//�r���[�s��B
		Matrix mProj;		//�v���W�F�N�V�����s��B
	};
	ConstantBuffer m_commonConstantBuffer;					//���b�V�����ʂ̒萔�o�b�t�@�B
	ConstantBuffer m_expandConstantBuffer;

	std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV> m_expandShaderResourceView = { nullptr };	//���[�U�[�g���V�F�[�_�[���\�[�X�r���[�B
	StructuredBuffer m_boneMatricesStructureBuffer;	//�{�[���s��̍\�����o�b�t�@�B
	std::vector< std::shared_ptr<SMesh> > m_meshes;
	
	DescriptorHeap m_descriptorHeap;					//�f�B�X�N���v�^�q�[�v�B
	std::shared_ptr<Skeleton> m_skeleton = nullptr;						//�X�P���g���B
	void* m_expandData = nullptr;//���b�V���B

};