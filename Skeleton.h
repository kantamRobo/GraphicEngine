#pragma once
#include <list>
#include <functional>
#include <vector>
#include <memory>
#include <WinUser.h>
#include "Matrix.h"
class Bone {
public:
	//�R���X�g���N�^
	Bone(const wchar_t* boneName,
		const EngineMath::Matrix& bindPose,
		const EngineMath::Matrix& invBindPose,
		int parentBoneNo,
		int boneId) :
		m_boneName(boneName),
		m_bindPose(bindPose),
		m_invBindPose(invBindPose),
		m_parentBoneNo(parentBoneNo),
		m_worldMatrix(bindPose),
		m_boneId(boneId)
	{

	}
		
	void SetLocalMatrix(const EngineMath::Matrix& m)
	{
		m_localMatrix = m;
	}

	//���[�J���s����擾
	const EngineMath::Matrix& GetLocalMatrix()const
	{
		return m_localMatrix;

	}
	//���[���h�s����擾
	const EngineMath::Matrix& GetWorldMatrix()const
	{
		return m_worldMatrix;
	}

	//�o�C���h�|�[�Y�̍s����擾
	const EngineMath::Matrix& GetBindPoseMatrix()const
	{
		return m_bindPose;
	}

	//�o�C���h�|�[�Y�̋t�s����擾
	const EngineMath::Matrix& GetInvBindPoseMatrix()const
	{
		return m_invBindPose;

	}

	//�e�̃{�[���ԍ����擾
	int GetParentBoneNo()const
	{
		return m_parentBoneNo;

	}

	//�{�[���ԍ����擾
	int GetNo()const
	{
		return m_boneId;
	}

	//�q����ǉ�

	void AddChild(Bone* bone)
	{
		m_children.push_back(bone);

	}

	//�q�����擾
	std::list<Bone*>& GetChildren()
	{
		return m_children;
	}

	const EngineMath::Matrix& GetOffsetLocalMatrix()const
	{
		return m_offsetLocalMatrix;
	}


	bool InitSkeleton(const char* tksFilePath)
	{
		//tks�t�@�C�������[�h����B
		if (m_tksFile.Load(tksFilePath)) {
			//�{�[���s����\�z����B
			BuildBoneMatrices();
			return true;
		}
		return false;
	}


/*!
	*@brief	���O�̎擾�B
	*/
const wchar_t* GetName() const
{
	return m_boneName.c_str();
}
/*!
*@brief	���̃{�[���̃��[���h��Ԃł̈ʒu�Ɖ�]�ƃX�P�[�����v�Z����B
*@param[out]	trans		���s�ړ��ʂ̊i�[��B
*@param[out]	rot			��]�ʂ̊i�[��B
*@param[out]	scale		�g�嗦�̊i�[��B
*/
void CalcWorldTRS(EngineMath::Vector3& trans, EngineMath::Quaternion& rot, EngineMath::Vector3& scale);
private:
	std::wstring	m_boneName;
	int		     m_parentBoneNo = -1;
	int			 m_boneId = -1;
	EngineMath::Matrix       m_bindPose;
	EngineMath::Matrix       m_invBindPose;
	EngineMath::Matrix       m_localMatrix;
	EngineMath::Matrix       m_worldMatrix;
	EngineMath::Matrix       m_offsetMatrix;
	EngineMath::Matrix			m_offsetLocalMatrix;
	EngineMath::Vector3      m_position;
	EngineMath::Vector3      m_scale;
	EngineMath::Quaternion   m_rotation;
	std::list<Bone*> m_children;
};

class Skeleton
{
public:
	using OnPostProcessSkeletonUpdate = std::function<void()>;

	Skeleton();
	~Skeleton();

	//�{�[���̃��[�J���s���ݒ�
	void SetBoneLocalMatrix(int boneNo, const EngineMath::Matrix& m)
	{
		if (boneNo > (int)m_bones.size() - 1)
		{
			MessageBoxA(nullptr, "boneNo is over m_bones.size() ", "error", MB_OK);
			return;
		}
		m_bones[boneNo]->SetLocalMatrix(m);
	}

	//�{�[���̐����擾
	int GetNumBones()const
	{
		return static_cast<int>(m_bones.size());
	}

	//������

	bool Init(const char* FilePath);

	bool IsInited()const
	{
		return m_isInited;
	}

	//�{�[���s��̍\�z
	void BuildBoneMatrices();

	//�{�[���̖��O����{�[���ԍ�������

	int  FindBoneID(const wchar_t* boneName)const
	{
		for (int i = 0; i < static_cast<int>(m_bones.size()); i++)
		{
			if (wcscmp(m_bones[i]->GetName(), boneName) == 0) {
				return i;
			}
		}
		//������Ȃ�����
		return -1;
	}

	/// <summary>
	/// �{�[���ԍ�����{�[�����擾�B
	/// </summary>
	/// <param name="boneNo">�{�[���ԍ�</param>
	/// <returns>�{�[��</returns>
	Bone* GetBone(int boneNo) const
	{
		return m_bones[boneNo].get();
	}

	/// <summary>
	/// �{�[���s��̐擪�A�h���X���擾�B
	/// </summary>
	/// <returns></returns>
	EngineMath::Matrix* GetBoneMatricesTopAddress() const
	{
		return m_boneMatrixs.get();
	}

	/// <summary>
	/// �A�j���[�V�������Đ�����Ă���}�[�N��t����B
	/// </summary>
	void SetMarkPlayAnimation()
	{
		m_isPlayAnimation = true;
	}

public:
	//�X�V
	void Update(const EngineMath::Matrix& mWorld);

	//�{�[���̃��[���h�s��̍X�V�֐�

	static void UpdateBoneWorldMatrix(Bone& bone, const EngineMath::Matrix& parentMatrix);

	private:
		static const int BONE_MAX = 512;
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bones;
		std::unique_ptr<EngineMath::Matrix[]>	m_boneMatrixs;
		bool m_isInited = false;
		bool m_isPlayAnimation = false;
	};

}