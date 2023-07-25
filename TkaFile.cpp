#include "TkaFile.h"
#include "stdafx.h"
#include <errno.h>
#include <istream>
#include <fstream>
#include "Animation.h"
void TkaFile::Load(const char* filePath)
{
	//FILEの代わりってなんだっけ・・・・
	std::ifstream ifs(filePath);
	AnimClipHeader header;
	auto* fp = std::fopen(filePath, "rb");
	std::fread(&header, sizeof(header), 1, fp);

	if (header.numAnimationEvent > 0)
	{
		if (header.numAnimationEvent > 0)
		{
			//アニメーションイベントがあるなら、イベント情報をロードする
			for (auto i = 0; i < static_cast<int>(header.numAnimationEvent); i++)
			{
				float invokeTime = 0.0f;
				std::fread(&invokeTime, sizeof(invokeTime), 1, fp);
				//イベントと名前の長さを読み込む
				std::uint32_t eventNameLength;
				std::fread(&eventNameLength, sizeof(eventNameLength), 1, fp);

				//イベント名をロードする
				static char eventName[256];
				static wchar_t wEventName[256];
				std::fread(eventName, eventNameLength + 1, 1, fp);
				static char eventName[256];
				static wchar_t wEventName[256];
				AnimationEvent animEvent; 
				animEvent.invokeTime = invokeTime;
				animEvent.eventName = eventName;
				m_animationEvents.push_back(std::move(animEvent));
			}

		}

		//キーフレームの情報をごそっとロード
		m_keyFrames.resize(header.numKey);

		std::fread(&m_keyFrames.front(), sizeof(KeyFrame) * header.numKey, 1, fp);

		std::fclose(fp);


	}
}
