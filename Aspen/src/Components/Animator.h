#pragma once
#include "Component.h"
#include "../Renderer/Texture.h"

class SpriteRenderer;

class ASPEN AnimationClip
{
private:
	std::string m_Name;
	std::vector<std::shared_ptr<Texture>> m_Frames;
	std::vector<std::shared_ptr<Texture>>::iterator m_CurrentFrame;
	SpriteRenderer* m_SpriteRenderer = nullptr;

	float m_Duration;
	float m_FrameTime;
	float m_ElapsedTime;
	
	friend class Animator;
private:
	void SetSpriteRenderer(SpriteRenderer* spriteRenderer);

public:
	AnimationClip(const std::string& name);
	
	void Start();
	void Update();

	void AddFrame(const std::shared_ptr<Texture>& texture);
	void AddFrame(std::string_view path);

	void SetDuration(float duration);
	void SetName(const std::string& name);

	//-------------------------------------------------
	//Getters
	std::shared_ptr<Texture> GetFrame() const { return *m_CurrentFrame; }
	unsigned int GetFramesAmount() const { return m_Frames.size(); }
	const std::string& GetName() const { return m_Name; }
	float GetDuration() const { return m_Duration; }

	//-------------------------------------------------
	//Serialization
	void Serialize(json& out) const;
	void Deserialize(json& in);
};

class ASPEN Animator : public Component
{
private:
	std::vector<AnimationClip> m_AnimationClips;
	size_t m_CurrentClip = 0;

	std::unordered_map<std::string, bool> m_BoolParameters;
	std::unordered_map<std::string, float> m_FloatParameters;
	std::unordered_map<std::string, int> m_IntParameters;
	//std::unordered_map<std::string> m_TriggerParameters;

	friend class Inspector;
private:
	void AddAnimation(const std::string& name);

	void AddBoolParameter(const std::string& name);
	void AddFloatParameter(const std::string& name);
	void AddIntegerParameter(const std::string& name);

public:
	Animator(GameObject* gameObject, Transform* transform);
	
	void Start() override;
	void Update() override;

	void PlayAnimation(const std::string& name);

	//----------------------------------------------------------
	//Setters
	void SetBool(const std::string& name, bool value);
	void SetFloat(const std::string& name, float value);
	void SetInteger(const std::string& name, int value);
	void SetTrigger(const std::string& name);
	
	//----------------------------------------------------------
	//Getters
    AnimationClip& GetAnimation(const std::string& name);
	//AnimationClip& GetCurrentAnimation() const { return &m_AnimationClips.at(m_CurrentClip); }

	//----------------------------------------------------------
	//Serialization
	void Serialize(json& out) const override;
	void Deserialize(json& in) override;
};