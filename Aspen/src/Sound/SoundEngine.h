#pragma once
#include <IrrKlang/irrKlang.h>
#include "../Math/Vector2.h"

class SoundEngine
{
private:
	inline static SoundEngine* s_Instance = nullptr;
	irrklang::ISoundEngine* m_Engine;
	
	friend class AudioListener;
private:
	void SetListenerPosition(const Vector2f& position);

public:
	SoundEngine();

	irrklang::ISound* Play(std::string_view filename, const Vector2f& position, bool playLooped = false, bool startPaused = false, bool tack = false, irrklang::E_STREAM_MODE streamMode = irrklang::ESM_AUTO_DETECT, bool enableSoundEffects = false);

	static SoundEngine& Get();

	~SoundEngine();
};

