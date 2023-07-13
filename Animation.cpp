#include "Animation.h"
#include "stdafx.h"
#include "Model.h"
#include "Matrix.h"
#include "Vector.h"
using namespace EngineMath;
namespace {
	//DCCツールでのアニメーション上での1フレームの経過時間(単位は秒)
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
		//補間完了。
		//現在の最終アニメーションコントローラへのインデックスが開始インデックスになる。
		m_startAnimationPlayController = GetLastAnimationControllerIndex();
		m_numAnimationPlayController = 1;
		m_interpolateTime = 1.0f;
	}

	for (int i = 0; i < m_numAnimationPlayController - 1; i++) {
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i);
		m_animationPlayController[index].UpdateAnimationplayController(deltaTime, this);
	}
	//最後のポーズだけ進めていく。
	int lastIndex = GetLastAnimationControllerIndex();
	m_animationPlayController[lastIndex].UpdateAnimationplayController(deltaTime, this);
}

EngineMath::Vector3 Animation::CalcFootstepDeltaValueInWorldSpace(EngineMath::Quaternion rotation, EngineMath::Vector3 scale) const
{
	auto footstepDeltaValueInWorldSpace = m_footstepDeltaValue;

	EngineMath::Matrix mBias = EngineMath::Matrix::Identity;
	mBias.MakeRotationX(Math::PI * -0.5f);
	mBias.Apply(footstepDeltaValueInWorldSpace);

	//フットステップの移動量を拡大する。
	footstepDeltaValueInWorldSpace.x *= scale.x;
	footstepDeltaValueInWorldSpace.y *= scale.y;
	footstepDeltaValueInWorldSpace.z *= scale.z;
	//フットステップの移動量を回す。
	rotation.Apply(footstepDeltaValueInWorldSpace);
	//フットステップの移動量をオイラー積分する。
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

	//グローバルポーズを計算していく
	int startIndex = m_startAnimationPlayController;

	for (int i = 0; i < m_numAnimationPlayController; i++)
	{
		int index = GetAnimationControllerIndex(startIndex, i);
		float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
		const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
		auto deltaValueFootStep = m_animationPlayController[index].GetFootStepDeltaValueOnUpdate();
		//footstepの移動量の補完
		m_footstepDeltaValue.Lerp(intepolateRate, m_footstepDeltaValue, deltaValueFootStep);

		for (int boneNo = 0; boneNo < numBone; boneNo++)
		{
			EngineMath::Matrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(intepolateRate,
				vGlobalPose[boneNo],
				*(EngineMath::Vector3*)m.m[3]);

			//平行移動成分を削除。
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;
			//拡大成分の補間。
			EngineMath::Vector3 vBoneScale;
			vBoneScale.x =(*(EngineMath::Vector3*)m.m[0]).Length();
			vBoneScale.y = (*(EngineMath::Vector3*)m.m[1]).Length();
			vBoneScale.z= (*(EngineMath::Vector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale);


			//拡大成分を除去。
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;

			//回転の補完
			EngineMath::Quaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//グローバルポーズをスケルトンに反映させていく。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {

		//拡大行列を作成。
		EngineMath::Matrix scaleMatrix;
		scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
		//回転行列を作成。
		EngineMath::Matrix rotMatrix;
		rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
		//平行移動行列を作成。
		EngineMath::Matrix transMat;
		transMat.MakeTranslation(vGlobalPose[boneNo]);

		//全部を合成して、ボーン行列を作成。
		EngineMath::Matrix boneMatrix;
		boneMatrix = scaleMatrix * rotMatrix;
		boneMatrix = boneMatrix * transMat;

		m_skeleton->SetBoneLocalMatrix(
			boneNo,
			boneMatrix
		);

	}

	//最終アニメーション以外は補間完了していたら除去していく。
	int numAnimationPlayController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
			//補間が終わっているのでアニメーションの開始位置を前にする。
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
	//ローカルポーズの更新をやっていく。
	UpdateLocalPose(deltaTime);

	//グローバルポーズを計算していく。
	UpdateGlobalPose();
}