#include "Animator.h"
#include "../GameObject/GameObject.h"
#include "../Log/Log.h"

#include "../Core/Time.h"

void AnimationClip::SetSpriteRenderer(SpriteRenderer* spriteRenderer)
{
	m_SpriteRenderer = spriteRenderer;
}

AnimationClip::AnimationClip(const std::string& name)
	: m_Name(name)
{
}

void AnimationClip::Start()
{
	m_CurrentFrame = m_Frames.begin();
	m_SpriteRenderer->SetSprite(*m_CurrentFrame);
	
	m_FrameTime = m_Duration / m_Frames.size();
	m_ElapsedTime = 0;

	m_LastFrameTime = std::chrono::steady_clock::now();
}

void AnimationClip::Update()
{
	//auto now = std::chrono::steady_clock::now();
	//m_ElapsedTime += std::chrono::duration_cast<std::chrono::seconds>(now - m_LastFrameTime).count();
	//m_LastFrameTime = std::chrono::steady_clock::now();
	m_ElapsedTime += Time::DeltaTime();
	if (m_ElapsedTime >= m_FrameTime)
	{
		m_ElapsedTime = 0;
		m_CurrentFrame++;

		if (m_CurrentFrame == m_Frames.end())
			m_CurrentFrame = m_Frames.begin();

		m_SpriteRenderer->SetSprite(*m_CurrentFrame);
	}
}

void AnimationClip::AddFrame(const std::shared_ptr<Texture>& texture)
{
	m_Frames.push_back(texture);
}

void AnimationClip::SetDuration(float duration)
{
	if (duration < 0)
		return;

	m_Duration = duration;
}


void Animator::AddAnimation(const std::string& name)
{
	m_AnimationClips.push_back(AnimationClip(name));
	//temporarily
	m_CurrentClip = m_AnimationClips.begin();
}

void Animator::AddBoolParameter(const std::string& name)
{
	m_BoolParameters.insert({ name, false });
}

void Animator::AddFloatParameter(const std::string& name)
{
	m_FloatParameters.insert({ name, 0.0f });
}

void Animator::AddIntegerParameter(const std::string& name)
{
	m_IntParameters.insert({ name, 0 });
}

Animator::Animator(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{
}

void Animator::Start()
{
	m_CurrentClip->SetSpriteRenderer(gameObject->GetComponent<SpriteRenderer>());
	m_CurrentClip->Start();
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

const AnimationClip& Animator::GetAnimation(const std::string& name)
{
	auto returnClip = std::find_if(m_AnimationClips.cbegin(), m_AnimationClips.cend(),
		[&](const AnimationClip& clip)
		{
			if (clip.GetName() == name)
				return true;
		});

	return *returnClip;
}
