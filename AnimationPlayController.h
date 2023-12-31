#pragma once
#include "Vector.h"
#include <utility>

#include "AnimationClip.h"
#include "Animation.h"
#include "Skeleton.h"
#include "stdafx.h"

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


	void InitAnimationPlayController(std::shared_ptr<Skeleton> skeleton, int footStepBoneNo);

	void ChangeAnimationClip(std::shared_ptr<AnimationClip> clip);

	void UpdateSAnimationplayController(float deltaTime, Animation* animation);

	
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

	void UpdateAnimationplayController(float deltaTime, Animation* animation);

	const std::vector<EngineMath::Matrix>& GetBoneLocalMatrix()const
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
	EngineMath::Vector3 GetFootStepDeltaValueOnUpdate() const
	{
		return m_footstepDeltaValue;
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
	void CalcBoneMatrixInRootBoneSpace(Bone& bone, EngineMath::Matrix parentMatrix);
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


	
	

private:
	std::shared_ptr<AnimationClip> m_animationClip = nullptr;
	int							   m_currentKeyFrameNoLastFrame = 0;
	int							   m_currentKeyFrameNo = 0;
	float						   m_time = 0.0f;
	std::vector<EngineMath::Matrix>			   m_boneMatrix;
	float						   m_interpolateTime;
	float						   m_interpolateEndTime;
	bool						   m_isPlaying = false;
	std::shared_ptr<Skeleton>	   m_skeleton = nullptr;
	EngineMath::Vector3					m_footstepDeltaValue = EngineMath::g_vec3Zero;	//�t�b�g�X�e�b�v�̈ړ��x�N�g���B
	EngineMath::Vector3						   m_footstepPos = EngineMath::g_vec3Zero;
	int							   m_footstepBoneNo = -1;

};

