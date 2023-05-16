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
	/// Update関数を呼び出したときの、footstepボーンの移動量を取得。
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
		*@brief	アニメーションイベントを起動する。
		*/
	void InvokeAnimationEvent(Animation* animation);
	/*!
	*@brief	ループ再生開始する時の処理。
	*/
	void StartLoop();
	/// <summary>
	/// ルートのボーン空間でのボーン行列を計算する。
	/// </summary>
	/// <param name="bone">計算する骨</param>
	/// <param name="parentMatrix">親の行列</param>
	void CalcBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
	/// <summary>
	/// ボーン行列をアニメーションクリップからサンプリングする。
	/// </summary>
	void SamplingBoneMatrixFromAnimationClip();
	/// <summary>
	/// ルートボーン空間での行列を計算する。
	/// </summary>
	void CalcBoneMatrixInRootBoneSpace();
	/// <summary>
	/// footstepボーンの変化量をサンプリングする。
	/// </summary>
	void SamplingDeltaValueFootstepBone();
	/// <summary>
	/// footstepボーンの座標を全体の骨から減算する。
	/// </summary>
	void SubtractFootstepbonePosFromAllBone();
	/// <summary>
	/// キーフレーム番号を進める。
	/// </summary>
	void ProgressKeyframeNo(float deltaTime);


	//アニメーションイベントを起動する
	void InvokeAnimationEvent(Animation* animation);
	//ループ再生するときの処理
	void StartLoop();

	//ルートのボーン空間でのボーン行列を計算する
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
	Vector3					m_footstepDeltaValue = g_vec3Zero;	//フットステップの移動ベクトル。
	Vector3						   m_footstepPos = g_vec3Zero;
	int							   m_footstepBoneNo = -1;

};

