#include "Skeleton.h"
#include "Matrix.h"
#include "Vector.h"

void Skeleton::Update(const Matrix& mWorld)
{
}

void Skeleton::UpdateBoneWorldMatrix(Bone& bone, const Matrix& parentMatrix)
{
	Matrix mBoneWorld;
	Matrix localMatrix = bone.GetLocalMatrix();
	mBoneWorld = localMatrix * parentMatrix;

	bone.SetWorldMatrix(mBoneWorld);
	for (auto childBone : bone.GetChildren()) {
		UpdateBoneWorldMatrix(*childBone, mBoneWorld);
	}
}

void CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale)
{
	Matrix mWorld = m_worldMatrix;
	//�s�񂩂�g�嗦���擾����B
	scale.x = mWorld.v[0].Length();
	scale.y = mWorld.v[1].Length();
	scale.z = mWorld.v[2].Length();
	m_scale = scale;
	//�s�񂩂畽�s�ړ��ʂ��擾����B
	trans.Set(mWorld.v[3]);
	m_positoin = trans;
	//�s�񂩂�g�嗦�ƕ��s�ړ��ʂ��������ĉ�]�ʂ��擾����B
	mWorld.v[0].Normalize();
	mWorld.v[1].Normalize();
	mWorld.v[2].Normalize();
	mWorld.v[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	rot.SetRotation(mWorld);
	m_rotation = rot;
}



void Skeleton::BuildBoneMatrices()
{
	m_tksFile.QueryBone([&](TksFile::SBone& tksBone) {
		//�o�C���h�|�[�Y�B
		Matrix bindPoseMatrix;
		memcpy(bindPoseMatrix.m[0], &tksBone.bindPose[0], sizeof(tksBone.bindPose[0]));
		memcpy(bindPoseMatrix.m[1], &tksBone.bindPose[1], sizeof(tksBone.bindPose[1]));
		memcpy(bindPoseMatrix.m[2], &tksBone.bindPose[2], sizeof(tksBone.bindPose[2]));
		memcpy(bindPoseMatrix.m[3], &tksBone.bindPose[3], sizeof(tksBone.bindPose[3]));
		bindPoseMatrix.m[0][3] = 0.0f;
		bindPoseMatrix.m[1][3] = 0.0f;
		bindPoseMatrix.m[2][3] = 0.0f;
		bindPoseMatrix.m[3][3] = 1.0f;

		//�o�C���h�|�[�Y�̋t�s��B
		Matrix invBindPoseMatrix;
		memcpy(invBindPoseMatrix.m[0], &tksBone.invBindPose[0], sizeof(tksBone.invBindPose[0]));
		memcpy(invBindPoseMatrix.m[1], &tksBone.invBindPose[1], sizeof(tksBone.invBindPose[1]));
		memcpy(invBindPoseMatrix.m[2], &tksBone.invBindPose[2], sizeof(tksBone.invBindPose[2]));
		memcpy(invBindPoseMatrix.m[3], &tksBone.invBindPose[3], sizeof(tksBone.invBindPose[3]));
		invBindPoseMatrix.m[0][3] = 0.0f;
		invBindPoseMatrix.m[1][3] = 0.0f;
		invBindPoseMatrix.m[2][3] = 0.0f;
		invBindPoseMatrix.m[3][3] = 1.0f;

		wchar_t boneName[256];
		mbstowcs(boneName, tksBone.name.get(), 256);
		BonePtr bone = std::make_unique<Bone>(
			boneName,
			bindPoseMatrix,
			invBindPoseMatrix,
			tksBone.parentNo,
			tksBone.no
		);
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		//�{�[���̃o���f�[�V�����`�F�b�N�B
		//maxScript�ł�肽���Ƃ���ł͂��邪�A�Ƃ肠�����������B
		auto it = std::find_if(m_bones.begin(), m_bones.end(), [&](auto& bone) {return wcscmp(boneName, bone->GetName()) == 0;  });
		if (it != m_bones.end()) {
			//�����̃{�[�������������B
			_bstr_t b(boneName);
			const char* c = b;
			TK_WARNING("�����̃{�[����������܂����B����̓���ł��B�f�[�^���C�����Ă��������B%s", c);
		}
#endif
		m_bones.push_back(std::move(bone));
		});
	for (auto& bone : m_bones) {
		if (bone->GetParentBoneNo() != -1) {
			m_bones.at(bone->GetParentBoneNo())->AddChild(bone.get());
			//���[�J���}�g���N�X���v�Z�B
			const Matrix& parentMatrix = m_bones.at(bone->GetParentBoneNo())->GetInvBindPoseMatrix();
			Matrix localMatrix;
			localMatrix = bone->GetBindPoseMatrix() * parentMatrix;
			bone->SetLocalMatrix(localMatrix);
		}
		else {
			bone->SetLocalMatrix(bone->GetBindPoseMatrix());
		}
	}


	//�{�[���s����m��
	m_boneMatrixs = std::make_unique<Matrix[]>(m_bones.size());
	m_isInited = true;

}