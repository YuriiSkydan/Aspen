#include "Animator.h"
#include "../Log/Log.h"

AnimationClip::AnimationClip(const std::string& name)
	: m_Name(name)
{
}

void AnimationClip::Start()
{
	m_CurrentFrame = m_Frames.begin();
	m_SpriteRenderer->SetSprite(*m_CurrentFrame);

	m_FrameTime = m_Frames.size() / m_Duration;
	m_ElapsedTime = 0;

	m_LastFrameTime = std::chrono::steady_clock::now();
}

void AnimationClip::Update()
{
	auto now = std::chrono::steady_clock::now();
	m_ElapsedTime += std::chrono::duration_cast<std::chrono::seconds>(now - m_LastFrameTime).count();
	m_LastFrameTime = std::chrono::steady_clock::now();

	if (m_ElapsedTime >= m_FrameTime)
	{	
		m_ElapsedTime = 0;
		m_CurrentFrame++;

		if (m_CurrentFrame == m_Frames.end())
			m_CurrentFrame = m_Frames.begin();

		m_SpriteRenderer->SetSprite(*m_CurrentFrame);
	}
}


Animator::Animator(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{
}

void Animator::Update()
{
	m_CurrentClip->Update();
}

void Animator::SetBool(const std::string& name, bool value)
{
	if (m_BoolParameters.find(name) == m_BoolParameters.end())
	{
		WARN("Parameter is not found {0}", name.c_str());
		return;
	}

	m_BoolParameters[name] = value;
}

void Animator::SetFloat(const std::string& name, float value)
{
	if (m_FloatParameters.find(name) == m_FloatParameters.end())
	{
		WARN("Parameter is not found {0}", name.c_str());
		return;
	}

	m_FloatParameters[name] = value;
}

void Animator::SetInteger(const std::string& name, int value)
{
	if (m_IntParameters.find(name) == m_IntParameters.end())
	{
		WARN("Parameter is not found {0}", name.c_str());
		return;
	}

	m_IntParameters[name] = value;
}