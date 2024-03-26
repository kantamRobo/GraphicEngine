#pragma once
#include <functional>
#include "tiny_gltf_loader.h"


/// <summary>
	/// �L�[�t���[���ɃN�G�����s���B
	/// </summary>
	/// <param name="query">�N�G���֐��B</param>
void QueryKeyFrames(std::function<void(const KeyFrame& keyFrame)> query)
{
	for (auto& keyFrame : m_keyFrames) {
		query(keyFrame);
	}
}


/// <summary>
/// �A�j���[�V�����C�x���g�ɃN�G�����s���B
/// </summary>
/// <param name="query">�N�G���֐�</param>
void QueryAnimationEvents(std::function<void(const AnimationEvent& animEvent) > query)
{
	for (auto& animEvent : m_animationEvents) {
		query(animEvent);
	}
}