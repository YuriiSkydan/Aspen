#pragma once
#include <unordered_map>
#include <list>

#include "Component.h"
#include "../Renderer/Texture.h"

class ASPEN AnimationClip
{
private:
	std::string m_Name;
	std::vector<Texture> m_Sprites;
};

class ASPEN Animator : public Component
{
private:
	std::list<AnimationClip> m_AnimationClips;
	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;
	std::unordered_map<std::string, int> m_IntParameters;
	//std::unordered_map<std::string> m_TriggerParameters;

public:
	Animator(GameObject* gameObject, Transform* transform);

	void SetBool(std::string_view name, bool value);
	void SetFloat(std::string_view name, float value);
	void SetIneger(std::string_view name, int value);
	void SetTrigger(std::string_view name);
};