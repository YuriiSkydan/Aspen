#pragma once
#include <unordered_map>
#include <chrono>
#include <list>

#include "Component.h"
#include "SpriteRenderer.h"

class ASPEN AnimationClip
{
private:
	std::string m_Name;
	SpriteRenderer* m_SpriteRenderer = nullptr;
	std::vector<std::shared_ptr<Texture>> m_Frames;
	std::vector<std::shared_ptr<Texture>>::iterator m_CurrentFrame;

	float m_Duration;

	float m_FrameTime;
	float m_ElapsedTime;

	std::chrono::high_resolution_clock::time_point m_LastFrameTime;
	
	friend class Animator;
private:
	void SetSpriteRenderer(SpriteRenderer* spriteRenderer);

public:
	AnimationClip(const std::string& name);
	
	void Start();
	void Update();

	void AddFrame(const std::shared_ptr<Texture>& texture);
	void SetDuration(float duration);
	void SetName(const std::string& name);

	std::shared_ptr<Texture> GetFrame() const { return *m_CurrentFrame; }
	unsigned int GetFramesAmount() const { return m_Frames.size(); }
	const std::string& GetName() const { return m_Name; }
	float GetDuration() const { return m_Duration; }
};

class ASPEN AnimationState
{
private:
	//std::unordered_map<std::pair<std::string, bool>, bool> m_BoolCondition;
	AnimationClip m_AnimationClip;
};

class ASPEN Animator : public Component
{
private:
	std::list<AnimationClip> m_AnimationClips;
	std::list<AnimationClip>::iterator m_CurrentClip;

	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;
	std::unordered_map<std::string, int> m_IntParameters;
	//std::unordered_map<std::string> m_TriggerParameters;
	friend class Inspector;
private:
	void AddFrameToAnimation(const Texture& newFrame);
	void AddAnimation(const std::string& name);

	void AddBoolParameter(const std::string& name);
	void AddFloatParameter(const std::string& name);
	void AddIntegerParameter(const std::string& name);

public:
	Animator(GameObject* gameObject, Transform* transform);
	
	void Start() override;
	void Update() override;

	void PlayAnimation(const std::string& name);

	void SetBool(const std::string& name, bool value);
	void SetFloat(const std::string& name, float value);
	void SetInteger(const std::string& name, int value);
	void SetTrigger(const std::string& name);

	const AnimationClip& GetAnimation(const std::string& name);
};