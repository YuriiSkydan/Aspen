#include "AudioListener.h"
#include "Transform.h"
#include "../SoundEngine.h"

AudioListener::AudioListener(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void AudioListener::Update()
{
	SoundEngine::Get().SetListenerPosition(transform->position);
}
