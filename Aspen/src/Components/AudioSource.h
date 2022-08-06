#pragma once
#include "Component.h"
#include "../SoundEngine.h"

class ASPEN AudioSource : public Component
{
private:
	std::string m_FileName;
	irrklang::ISound* m_Sound;

public:
	AudioSource(GameObject* gameObject, Transform* transform);

	void Start() override;
	void Update() override;

	void SetFilepath();

	~AudioSource();
};