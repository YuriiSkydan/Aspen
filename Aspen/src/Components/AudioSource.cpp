#include "AudioSource.h"
#include "Transform.h"

using namespace irrklang;

AudioSource::AudioSource(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void AudioSource::Start()
{
	m_Sound = SoundEngine::Get().Play(m_Filename, transform->position, false, false, true);

	if (m_Sound != nullptr)
	{
		m_Sound->setMaxDistance(m_MaxDistance);
		m_Sound->setMinDistance(m_MinDistance);
		m_Sound->setIsLooped(m_IsLooped);
	}
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

void AudioSource::SetFilename(std::string_view filename)
{
	m_Filename = filename;
}

void AudioSource::SetMaxDistance(float distance)
{
	m_MaxDistance = distance;

	if (m_Sound != nullptr)
		m_Sound->setMaxDistance(m_MaxDistance);
}

void AudioSource::SetMinDistance(float distance)
{
	m_MinDistance = distance;

	if (m_Sound != nullptr)
		m_Sound->setMinDistance(m_MinDistance);
}

void AudioSource::SetLooped(bool value)
{
	m_IsLooped = value;

	if (m_Sound != nullptr)
		m_Sound->setIsLooped(m_IsLooped);
}

void AudioSource::Serialize(json& out) const
{
	Component::Serialize(out["AudioSource"]);

	out["AudioSource"] =
	{
		{ "Filename",  GetFilename() },
		{ "MinDistance",  GetMinDistance()},
		{ "MaxDistance",  GetMaxDistance()},
		{ "IsLooped",  GetIsLooped()}
	};
}

void AudioSource::Deserialize(json& in)
{
	Component::Deserialize(in);

	SetFilename(in["Filename"]);
	SetMinDistance(in["MinDistance"]);
	SetMinDistance(in["MaxDistance"]);
	SetLooped(in["IsLooped"]);
}

AudioSource::~AudioSource()
{
	if (m_Sound != nullptr)
		m_Sound->drop();
}
