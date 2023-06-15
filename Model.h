#pragma once
#include "MeshParts.h"
#include "Skeleton.h"
#include "Vector.h"
#include "Matrix.h"
class IShaderResource;

//���f���̏����
enum EnModelUpAxis {
	enModelUpAxisY,		//���f���̏������Y���B
	enModelUpAxisZ,		//���f���̏������Z���B
};

//���f���̏������f�[�^
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;
	const char* m_vsEntryPointFunc = "VSMain";
	const char* m_vsSkinEntrypointFunc = "VSMain";
	const char* m_psEntryPointFunc = "PSMain";
	const char* m_fxFilePath = nullptr;
	void* m_expandConstantBuffer = nullptr;
	int m_expandConstantBufferSize = 0;

	std::array<DXGI_FORMAT, MAX_RENDERING_TARGET> m_colorBufferFormat = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B

};

class Model
{
public:
	/// <summary>
	/// tkm�t�@�C�����珉�����B
	/// </summary>
	/// <param name="initData">�������f�[�^</param>
	void InitModel(const ModelInitData& initData);

	/// <summary>
	/// ���[���h�s����v�Z���āA�����o�ϐ���m_worldMatrix���������񂷂�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	void Draw(RenderContext& renderContext);

	/// <summary>
	/// �`��(�J�����w���)
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="camera">�J����</param>
	void Draw(RenderContext& renderContext, Camera& camera);
	/// <summary>
	/// �`��(�J�����s��w���)
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="viewMatrix">�r���[�s��</param>
	/// <param name="projMatrix">�v���W�F�N�V�����s��</param>
	void Draw(RenderContext& renderContext, const Matrix& viewMatrix, const Matrix& projMatrix);
	void DrawInstancing(std::shared_ptr<RenderContext> rc, int numInstance);
	/// <summary>
	/// �C���X�^���V���O�`��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="numInstance">�C���X�^���X�̐�</param>
	void DrawInstancing(RenderContext& renderContext, int numInstance);
	/// <summary>
	/// ���[���h�s����擾�B
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
	/// <summary>
	/// ���b�V���ɑ΂��Ė₢���킹���s���B
	/// </summary>
	/// <param name="queryFunc">�₢���킹�֐�</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		m_meshParts.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
	}
	/// <summary>
	/// �A���x�h�}�b�v��ύX�B
	/// </summary>
	/// <remarks>
	/// ���̊֐����Ăяo���ƃf�B�X�N���v�^�q�[�v�̍č\�z���s���邽�߁A�������ׂ�������܂��B
	/// ���t���[���Ăяo���K�v���Ȃ��ꍇ�͌Ăяo���Ȃ��悤�ɂ��Ă��������B
	/// </remarks>
	/// <param name="materialName">�ύX�������}�e���A���̖��O</param>
	/// <param name="albedoMap">�A���x�h�}�b�v</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
	/// <summary>
	/// TKM�t�@�C�����擾�B
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *m_tkmFile;
	}
	/// <summary>
	/// ����������Ă��邩����B
	/// </summary>
	/// <returns></returns>
	bool IsInited() const
	{
		return m_isInited;
	}

	/// <summary>
	/// ���[���h�s����v�Z����B
	/// </summary>
	/// <remark>
	/// ���̊֐���UpdateWorldMatrix�֐��̒�����g���Ă��܂��B
	/// Model�N���X�̎g�p�ɉ��������[���h�s��̌v�Z���s�������ꍇ�A
	/// �{�֐��𗘗p����ƌv�Z���邱�Ƃ��ł��܂��B
	/// </remark>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦�B</param>
	/// <returns></returns>
	/// 
	Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mWorld;
		Matrix mBias;
		if (m_modelUpAxis == enModelUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);

		mWorld = mBias * mScale * mRot * mTrans;
		return mWorld;


	}
	private:

private:
	bool m_isInited = false;						// ����������Ă���H
	Matrix m_world;									// ���[���h�s��B
	TkmFile* m_tkmFile;								// tkm�t�@�C���B
	Skeleton m_skeleton;							// �X�P���g���B
	MeshParts m_meshParts;							// ���b�V���p�[�c�B
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;



};
