#pragma once
#include <iostream> // for debug

class Transform;
class GameObject;

class Component
{
public:
	//GameObject* gameObject;
	Transform* transform;

	bool m_IsEnabled = true;

	friend class GameObject;
	friend class Inspector;

private:
	virtual void UpdateGui() {};

public:
	Component(GameObject* gameObject, Transform* transform);
	Component(const Component& other) = delete;
	const Component& operator=(const Component& other);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void OnDestroy() {}
	bool IsEnabled() const { return m_IsEnabled; }

	virtual ~Component()
	{
		std::cout << "Component destructor!!!\n";
	}
};