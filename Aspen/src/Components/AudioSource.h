#pragma once
#include "Component.h"
#include "../Sound/SoundEngine.h"

class ASPEN AudioSource : public Component
{
private:
	std::string m_Filename;
	irrklang::ISound* m_Sound = nullptr;

	float m_MaxDistance = 5.0f;
	float m_MinDistance = 0.0f;
	
	bool m_IsLooped = false;
	bool m_StartPaused = false;
public:
	AudioSource(GameObject* gameObject, Transform* transform);

	void Start() override;
	void Update() override;

	//------------------------------------------------------------
	//Setters
	void SetFilename(std::string_view filename);
	void SetMaxDistance(float distance);
	void SetMinDistance(float distance);
	void SetLooped(bool value);
	void SetStartPaused(bool value);

	//------------------------------------------------------------
	//Getters
	std::string_view GetFilename() const { return m_Filename; }
	float GetMinDistance() const { return m_MinDistance; }
	float GetMaxDistance() const { return m_MaxDistance; }
	bool GetStartPaused() const { return m_StartPaused; }
	bool GetIsLooped() const { return m_IsLooped; }

	//------------------------------------------------------------
	//Serialization
	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

	~AudioSource();
};