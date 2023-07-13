#include "Animation.h"
#include "stdafx.h"
#include "Model.h"
#include "Matrix.h"
#include "Vector.h"
using namespace EngineMath;
namespace {
	//DCC�c�[���ł̃A�j���[�V������ł�1�t���[���̌o�ߎ���(�P�ʂ͕b)
	const float DELTA_SEC_PER_FRAME_ON_DCC_TOOL = 1.0f / 30.0f;
}

void Animation::InitAnimation(Skeleton& skeleton, AnimationClip* animClips, int numAnimClip)
{
	m_skeleton = &skeleton;
	for (int i = 0; i < numAnimClip; i++)
	{
		m_animationClips.push_back(&animClips[i]);
	}

	int footstepBoneNo = -1;
	int numBone = m_skeleton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++)
	{
		auto bone = m_skeleton->GetBone(boneNo);
		if (wcscmp(bone->GetName(), L"footstep") == 0)
		{
			footstepBoneNo = boneNo;
			break;
		}
	}

	for (auto& ctr : m_animationPlayController)
	{
		ctr.InitAnimationPlayController(m_skeleton, footstepBoneNo);
	}
	Play(0);

	m_isInited = true;
}

void Animation::UpdateLocalPose(float deltaTime)
{
	m_interpolateTime += deltaTime;
	if (m_interpolateTime >= 1.0f)
	{
		//��Ԋ����B
		//���݂̍ŏI�A�j���[�V�����R���g���[���ւ̃C���f�b�N�X���J�n�C���f�b�N�X�ɂȂ�B
		m_startAnimationPlayController = GetLastAnimationControllerIndex();
		m_numAnimationPlayController = 1;
		m_interpolateTime = 1.0f;
	}

	for (int i = 0; i < m_numAnimationPlayController - 1; i++) {
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
		m_animationPlayController[index].UpdateAnimationplayController(deltaTime, this);
	}
	//�Ō�̃|�[�Y�����i�߂Ă����B
	int lastIndex = GetLastAnimationControllerIndex();
	m_animationPlayController[lastIndex].UpdateAnimationplayController(deltaTime, this);
}

EngineMath::Vector3 Animation::CalcFootstepDeltaValueInWorldSpace(EngineMath::Quaternion rotation, EngineMath::Vector3 scale) const
{
	auto footstepDeltaValueInWorldSpace = m_footstepDeltaValue;

	EngineMath::Matrix mBias = EngineMath::Matrix::Identity;
	mBias.MakeRotationX(Math::PI * -0.5f);
	mBias.Apply(footstepDeltaValueInWorldSpace);

	//�t�b�g�X�e�b�v�̈ړ��ʂ��g�傷��B
	footstepDeltaValueInWorldSpace.x *= scale.x;
	footstepDeltaValueInWorldSpace.y *= scale.y;
	footstepDeltaValueInWorldSpace.z *= scale.z;
	//�t�b�g�X�e�b�v�̈ړ��ʂ��񂷁B
	rotation.Apply(footstepDeltaValueInWorldSpace);
	//�t�b�g�X�e�b�v�̈ړ��ʂ��I�C���[�ϕ�����B
	float t = m_deltaTimeOnUpdate / DELTA_SEC_PER_FRAME_ON_DCC_TOOL;
	footstepDeltaValueInWorldSpace *= t;
	return footstepDeltaValueInWorldSpace;

}

void Animation::UpdateGlobalPose()
{
	int numBone = m_skeleton->GetNumBones();

	EngineMath::Quaternion*	qGlobalPose = (EngineMath::Quaternion*)alloca(sizeof(EngineMath::Quaternion) * numBone);
	EngineMath::Vector3* vGlobalPose = (EngineMath::Vector3*)alloca(sizeof(EngineMath::Vector3) * numBone);
	EngineMath::Vector3* vGlobalScale = (EngineMath::Vector3*)alloca(sizeof(EngineMath::Vector3) * numBone);

	m_footstepDeltaValue = g_vec3Zero;
	for (int i = 0; i < numBone; i++) {
		qGlobalPose[i] = EngineMath::Quaternion::Identity;
		vGlobalPose[i] = EngineMath::Vector3::Zero;
		vGlobalScale[i] = EngineMath::Vector3::One;
	}

	//�O���[�o���|�[�Y���v�Z���Ă���
	int startIndex = m_startAnimationPlayController;

	for (int i = 0; i < m_numAnimationPlayController; i++)
	{
		int index = GetAnimationControllerIndex(startIndex, i);
		float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
		const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
		auto deltaValueFootStep = m_animationPlayController[index].GetFootStepDeltaValueOnUpdate();
		//footstep�̈ړ��ʂ̕⊮
		m_footstepDeltaValue.Lerp(intepolateRate, m_footstepDeltaValue, deltaValueFootStep);

		for (int boneNo = 0; boneNo < numBone; boneNo++)
		{
			EngineMath::Matrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(intepolateRate,
				vGlobalPose[boneNo],
				*(EngineMath::Vector3*)m.m[3]);

			//���s�ړ��������폜�B
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;
			//�g�听���̕�ԁB
			EngineMath::Vector3 vBoneScale;
			vBoneScale.x =(*(EngineMath::Vector3*)m.m[0]).Length();
			vBoneScale.y = (*(EngineMath::Vector3*)m.m[1]).Length();
			vBoneScale.z= (*(EngineMath::Vector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale);


			//�g�听���������B
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;

			//��]�̕⊮
			EngineMath::Quaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//�O���[�o���|�[�Y���X�P���g���ɔ��f�����Ă����B
	for (int boneNo = 0; boneNo < numBone; boneNo++) {

		//�g��s����쐬�B
		EngineMath::Matrix scaleMatrix;
		scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
		//��]�s����쐬�B
		EngineMath::Matrix rotMatrix;
		rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
		//���s�ړ��s����쐬�B
		EngineMath::Matrix transMat;
		transMat.MakeTranslation(vGlobalPose[boneNo]);

		//�S�����������āA�{�[���s����쐬�B
		EngineMath::Matrix boneMatrix;
		boneMatrix = scaleMatrix * rotMatrix;
		boneMatrix = boneMatrix * transMat;

		m_skeleton->SetBoneLocalMatrix(
			boneNo,
			boneMatrix
		);

	}

	//�ŏI�A�j���[�V�����ȊO�͕�Ԋ������Ă����珜�����Ă����B
	int numAnimationPlayController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
			//��Ԃ��I����Ă���̂ŃA�j���[�V�����̊J�n�ʒu��O�ɂ���B
			m_startAnimationPlayController = index;
			numAnimationPlayController = m_numAnimationPlayController - i;
		}
	}
	m_numAnimationPlayController = numAnimationPlayController;
}


void Animation::Progress(float deltaTime)
{
	if (m_numAnimationPlayController == 0) {
		return;
	}
	m_deltaTimeOnUpdate = deltaTime;
	//���[�J���|�[�Y�̍X�V������Ă����B
	UpdateLocalPose(deltaTime);

	//�O���[�o���|�[�Y���v�Z���Ă����B
	UpdateGlobalPose();
}