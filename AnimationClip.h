#pragma once
#include <memory>
#include <vector>
#include <string>
struct KeyFrame {
	uint32_t boneIndex;
	float time;
	EngineMath::Matrix transform;

};

class AnimationEvent
{
public:

	float GetInvokeTime()const
	{
		return m_invokeTime;
	}

	//イベント名を取得

	const wchar_t* GetEventName()const
	{
		return m_eventName.c_str();
	}

	void SetInvokeTime(float time)
	{
		m_invokeTime = time;
	}

	//イベント名を設定
	void SetEventName(const wchar_t* name)
	{
		m_eventName = name;
	}

	//イベントが発生済みか判定
	bool IsInvoked()const
	{
		return m_isInvoked;
	}

	//イベントが発生済みのフラグを設定する
	void SetInvokedFlag(bool flag)
	{
		m_isInvoked = flag;
	}

private:
	bool m_isInvoked = false;
	float m_invokeTime = 0.0f;
	std::wstring m_eventName;
};

class AnimationClip
{
public:
	using keyFramePtrList = std::vector<KeyFrame*>;
	~AnimationClip();
	void Load(const char* filePath);

	void BuildKeyFramesAndAnimationEvents();

	/// <summary>
	/// キーフレームとアニメーションイベントを構築する。
	/// </summary>
	/// <remarks>
	/// ロードが終わったときに呼び出してください。
	/// </remarks>
	void BuildKeyFramesandAnimationEvents();

	bool IsLoop()const
	{
		return m_isLoop;
	}

	//ループフラグを設定する
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}

	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}
	/*!
		*@brief	クリップ名を取得。
		*/
	const wchar_t* GetName() const
	{
		return m_clipName.c_str();
	}
	/*!
	*@brief	アニメーションイベントを取得。
	*/
	std::unique_ptr<AnimationEvent[]>& GetAnimationEvent()
	{
		return m_animationEvent;
	}

	/// <summary>
	/// アニメーションイベントの数を取得
	/// </summary>
	/// <returns></returns>
	int GetNumAnimationEvent() const
	{
		return m_tkaFile.GetNumAnimationEvent();
	}

private:
	using KeyframePtr = std::unique_ptr<KeyFrame>;
	std::wstring m_clipName;
	bool m_isLoop = false;
	std::vector<KeyframePtr> m_keyframes;
	std::vector<std::shared_ptr<keyFramePtrList>> m_keyFramePtrListArray;
	std::unique_ptr<AnimationEvent[]> m_animationEvent;

	std::shared_ptr<keyFramePtrList> 	m_topBoneKeyFramList = nullptr;



};
using AnimationClipPtr = std::unique_ptr<AnimationClip>;
