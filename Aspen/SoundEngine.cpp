#include <string_view>
#include "SoundEngine.h"
#include "src/Log/Log.h"

using namespace irrklang;

void SoundEngine::SetListenerPosition(const Vector2f& position)
{
	if (m_Engine != nullptr)
	{
		m_Engine->setListenerPosition({ position.x, 0.0f, position.y }, { 0.0f, 0.0f, 1.0f });
	}
}

SoundEngine::SoundEngine()
{
	m_Engine = createIrrKlangDevice();

	if (m_Engine == nullptr)
	{
		ERROR("Failed to create sound engine.");
	}
}

irrklang::ISound* SoundEngine::Play(std::string_view filename, const Vector2f& position, bool playLooped, bool startPaused, bool track, E_STREAM_MODE streamMode, bool enableSoundEffects)
{
	if (m_Engine != nullptr)
	{
		vec3d soundPosition = vec3d(position.x, 0.0f, position.y);
		ISound* sound = m_Engine->play3D(filename.data(), soundPosition, playLooped, startPaused, track, streamMode, enableSoundEffects);
		return sound;
	}

	return nullptr;
}

SoundEngine& SoundEngine::Get()
{
	static SoundEngine soundEngine;
	s_Instance = &soundEngine;

	return *s_Instance;
}

SoundEngine::~SoundEngine()
{
	if (m_Engine != nullptr)
	{
		m_Engine->drop();
	}
}
