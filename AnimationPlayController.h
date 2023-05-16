#pragma once
#include "Vector.h"
#include <utility>

class Skeleton;
class AnimationClip;
class Animation;
class Bone;

class AnimationPlayController
{
public:
	AnimationPlayController()
	{

	}

	~AnimationPlayController()
	{
	}
	void InitAnimationPlayController(Skeleton* skeleton, int footStepBoneNo);


	void ChangeAnimationClip(AnimationClip* clip);

	void SetInterpolateTime(float interpolateTime)
	{
		if (interpolateTime < 0.0f)
		{
			return;
		}

		m_interpolateEndTime = interpolateTime;
		m_interpolateTime = 0.0f;
	}

	float GetInterpolateEndTime()const
	{
		if (m_interpolateEndTime <= 0.0f)
		{
			return 1.0f;
		}
		return std::min(1.0f, m_interpolateTime / m_interpolateEndTime);
	}

	void UpdateSAnimationplayController(float deltaTime, Animation* animation);

	const std::vector<Matrix>& GetBoneLocalMatrix()const
	{
		return m_boneMatrix;
	}

	std::shared_ptr<AnimationClip> GetAnimClip()const
	{
		return m_animationClip;
	}

	bool IsPlaying()const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// Update�֐����Ăяo�����Ƃ��́Afootstep�{�[���̈ړ��ʂ��擾�B
	/// </summary>
	/// <returns></returns>
	Vector3 GetFootStepDeltaValueOnUpdate() const
	{
		return m_footstepDeltaValue;
	}
	void SetInterpolateTime(float interpolateTime)
	{
		if (interpolateTime < 0.0f)
		{
			return;
		}
		m_interpolateEndTime

	}

private:
	/*!
		*@brief	�A�j���[�V�����C�x���g���N������B
		*/
	void InvokeAnimationEvent(Animation* animation);
	/*!
	*@brief	���[�v�Đ��J�n���鎞�̏����B
	*/
	void StartLoop();
	/// <summary>
	/// ���[�g�̃{�[����Ԃł̃{�[���s����v�Z����B
	/// </summary>
	/// <param name="bone">�v�Z���鍜</param>
	/// <param name="parentMatrix">�e�̍s��</param>
	void CalcBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
	/// <summary>
	/// �{�[���s����A�j���[�V�����N���b�v����T���v�����O����B
	/// </summary>
	void SamplingBoneMatrixFromAnimationClip();
	/// <summary>
	/// ���[�g�{�[����Ԃł̍s����v�Z����B
	/// </summary>
	void CalcBoneMatrixInRootBoneSpace();
	/// <summary>
	/// footstep�{�[���̕ω��ʂ��T���v�����O����B
	/// </summary>
	void SamplingDeltaValueFootstepBone();
	/// <summary>
	/// footstep�{�[���̍��W��S�̂̍����猸�Z����B
	/// </summary>
	void SubtractFootstepbonePosFromAllBone();
	/// <summary>
	/// �L�[�t���[���ԍ���i�߂�B
	/// </summary>
	void ProgressKeyframeNo(float deltaTime);


	//�A�j���[�V�����C�x���g���N������
	void InvokeAnimationEvent(Animation* animation);
	//���[�v�Đ�����Ƃ��̏���
	void StartLoop();

	//���[�g�̃{�[����Ԃł̃{�[���s����v�Z����
	void CalcBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
private:
	std::shared_ptr<AnimationClip> m_animationClip = nullptr;
	int							   m_currentKeyFrameNoLastFrame = 0;
	int							   m_currentKeyFrameNo = 0;
	float						   m_time = 0.0f;
	std::vector<Matrix>			   m_boneMatrix;
	float						   m_interpolateTime;
	float						   m_interpolateEndTime;
	bool						   m_isPlaying = false;
	std::shared_ptr<Skeleton>	   m_skeleton = nullptr;
	Vector3					m_footstepDeltaValue = g_vec3Zero;	//�t�b�g�X�e�b�v�̈ړ��x�N�g���B
	Vector3						   m_footstepPos = g_vec3Zero;
	int							   m_footstepBoneNo = -1;

};

