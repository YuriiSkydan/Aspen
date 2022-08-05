#include "AudioSource.h"
#include "Transform.h"

using namespace irrklang;

AudioSource::AudioSource(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void AudioSource::Start()
{
	m_Sound = SoundEngine::Get().Play("Assets/ophelia.mp3", transform->position, false, false, true);
}

void AudioSource::Update()
{
	Vector2f position = transform->position;
	if (m_Sound->getPosition().X != position.x ||
		m_Sound->getPosition().Z != position.y)
	{
		m_Sound->setPosition({ position.x, 0.0f, position.y });
	}
}