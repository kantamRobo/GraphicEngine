#pragma once
#include <functional>
#include "tiny_gltf_loader.h"


/// <summary>
	/// キーフレームにクエリを行う。
	/// </summary>
	/// <param name="query">クエリ関数。</param>
void QueryKeyFrames(std::function<void(const KeyFrame& keyFrame)> query)
{
	for (auto& keyFrame : m_keyFrames) {
		query(keyFrame);
	}
}


/// <summary>
/// アニメーションイベントにクエリを行う。
/// </summary>
/// <param name="query">クエリ関数</param>
void QueryAnimationEvents(std::function<void(const AnimationEvent& animEvent) > query)
{
	for (auto& animEvent : m_animationEvents) {
		query(animEvent);
	}
}