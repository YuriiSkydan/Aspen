#include "Component.h"

//Component::Component(std::shared_ptr<GameObject> gameObject, std::shared_ptr<Transform> transform)
//	: m_GameObject(gameObject), m_Transform(transform)
//{
//}

Component::Component(GameObject* gameObject, Transform* transform)
	: gameObject(gameObject), transform(transform)
{
}

const Component& Component::operator=(const Component& other)
{
	m_IsEnabled = other.m_IsEnabled;
	return *this;
}

void Component::SetEnabled(bool enabled)
{
	if (m_IsEnabled == true && enabled == false)
	{
		m_IsEnabled = enabled;
		OnDisabled();
	}
	else if(m_IsEnabled == false && enabled == true)
	{
		m_IsEnabled = enabled;
		OnEnabled();
	}
}

bool Component::operator==(const Component& other)
{
	return ((transform == other.transform) && (gameObject == other.gameObject));
}

bool Component::operator!=(const Component& other)
{
	return ((transform != other.transform) && (gameObject != other.gameObject));
}
