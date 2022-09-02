#include "Animator.h"
#include "../Scene/Scene.h"
#include "../Core/Time.h"

void AnimationClip::SetSpriteRenderer(SpriteRenderer* spriteRenderer)
{
	m_SpriteRenderer = spriteRenderer;
}

AnimationClip::AnimationClip(const std::string& name)
	: m_Name(name), m_Duration(0.0f), m_FrameTime(0.0f), m_ElapsedTime(0.0f)
{
	m_CurrentFrame = m_Frames.begin();
}

void AnimationClip::Start()
{
	m_CurrentFrame = m_Frames.begin();

	if (m_Frames.size() != 0)
	{
		m_SpriteRenderer->SetSprite(*m_CurrentFrame);

		m_FrameTime = m_Duration / m_Frames.size();
		m_ElapsedTime = 0;
	}
}

void AnimationClip::Update()
{
	m_ElapsedTime += Time::DeltaTime();
	if (m_ElapsedTime >= m_FrameTime)
	{
		m_ElapsedTime = 0;
		m_CurrentFrame++;

		if (m_CurrentFrame == m_Frames.end())
			m_CurrentFrame = m_Frames.begin();

		if (m_SpriteRenderer != nullptr)
			m_SpriteRenderer->SetSprite(*m_CurrentFrame);
	}
}

void AnimationClip::AddFrame(const std::shared_ptr<Texture>& texture)
{
	m_Frames.push_back(texture);
}

void AnimationClip::AddFrame(std::string_view path)
{
	std::shared_ptr<Texture> newFrame;
	TextureLibrary::Get()->GetTexture(path.data(), newFrame);
	m_Frames.push_back(newFrame);
}

void AnimationClip::SetDuration(float duration)
{
	if (duration < 0)
		return;

	m_Duration = duration;
}

void AnimationClip::SetName(const std::string& name)
{
	m_Name = name;
}

void AnimationClip::Serialize(json& out) const
{
	out =
	{
		{ "Name", m_Name },
		{ "Duration", m_Duration },
		{ "FramesAmount", m_Frames.size() }
	};

	for (size_t i = 0; i < m_Frames.size(); i++)
		out[std::to_string(i)] = m_Frames[i]->GetPath();
}

void AnimationClip::Deserialize(json& in)
{
	m_Name = in["Name"];
	m_Duration = in["Duration"];

	size_t framesAmount = in["FramesAmount"];
	for (size_t i = 0; i < framesAmount; i++)
	{
		std::string path = in[std::to_string(i)];
		std::shared_ptr<Texture> frame;
		TextureLibrary::Get()->GetTexture(path, frame);
		m_Frames.push_back(frame);
	}
}


void Animator::AddAnimation(const std::string& name)
{
	SpriteRenderer* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	m_AnimationClips.push_back(AnimationClip(name));
	m_AnimationClips.back().SetSpriteRenderer(spriteRenderer);
	//temporarily
	m_CurrentClip = 0;
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
{ }

void Animator::Start()
{
	if (m_AnimationClips.size() != 0)
	{
		m_AnimationClips[m_CurrentClip].SetSpriteRenderer(gameObject->GetComponent<SpriteRenderer>());
		m_AnimationClips[m_CurrentClip].Start();
	}
}

void Animator::Update()
{
	if (m_AnimationClips.size() != 0)
	{
		m_AnimationClips[m_CurrentClip].Update();
	}
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

void Animator::PlayAnimation(const std::string& name)
{
	if (m_AnimationClips[m_CurrentClip].GetName() != name)
	{
		for (size_t i = 0; i < m_AnimationClips.size(); i++)
		{
			if (m_AnimationClips[i].GetName() == name)
			{
				m_CurrentClip = i;
				m_AnimationClips[m_CurrentClip].SetSpriteRenderer(GetComponent<SpriteRenderer>());
				m_AnimationClips[m_CurrentClip].Start();
				break;
			}
		}
	}
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

AnimationClip& Animator::GetAnimation(const std::string& name)
{
	auto returnClip = std::find_if(m_AnimationClips.begin(), m_AnimationClips.end(),
		[&](const AnimationClip& clip)
		{
			if (clip.GetName() == name)
				return true;
		});

	return *returnClip;
}

void Animator::Serialize(json& out) const
{
	Component::Serialize(out["Animator"]);

	out["Animator"] =
	{
		{ "ClipsAmount", m_AnimationClips.size() }
	};

	for (size_t i = 0; i < m_AnimationClips.size(); i++)
		m_AnimationClips[i].Serialize(out["Animator"][std::to_string(i)]);
}

void Animator::Deserialize(json& in)
{
	Component::Deserialize(in);

	size_t clipsAmount = in["ClipsAmount"];
	for (size_t i = 0; i < clipsAmount; i++)
	{
		AnimationClip newClip("New Clip");
		newClip.Deserialize(in[std::to_string(i)]);
		newClip.SetSpriteRenderer(GetComponent<SpriteRenderer>());
		m_AnimationClips.push_back(newClip);
		m_AnimationClips.back().Start();
	}
}