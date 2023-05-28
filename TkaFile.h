#pragma once
class TkaFile
{

	struct AnimClipHeader
	{
		std::uint32_t numKey;
		std::uint32_t numAnimationEvent;
	};

	struct AnimationEvent {
		float invokeTime;
		std::string eventName;
	};

	struct KeyFrame {
		std::uint32_t boneIndex;
		float time;
		Vector3 transform[4];

	};

	void Load(const char* filePath);

	void QueryAnimationEvents(std::function<void(const AnimationEvent&) animEvent) > query)
	{
		for (auto& animEvent : m_animationEvents)
		{
			query(animEvent);
		}
	}

	void QueryKeyFrames(std::function<void(const KeyFrame& keyframe)> query)
	{
		for (auto& keyFrame : m_keyFrames)
		{
			query(keyframe);
		}

	}

	/// <summary>
	/// �A�j���[�V�����C�x���g�̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumAnimationEvent() const
	{
		return static_cast<int>(m_animationEvents.size());
	}
	/// <summary>
	/// �L�[�t���[���̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumKeyFrame() const
	{
		return static_cast<int>(m_keyFrames.size());
	}

private:
	std::vector<AnimationEvent> m_animationEvents;
	std::vector<KeyFrame> m_keyFrames;
};

