#pragma once
#include <iostream> // for debug

#include "../Core/Core.h"

class Transform;
class GameObject;

class ASPEN Component
{
public:
	GameObject* gameObject;
	Transform* transform;

	bool m_IsEnabled = true;

	friend class GameObject;
	friend class Inspector;

protected:
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

	template<typename T>
	T* GetComponent();

	template<typename T>
	bool HasComponent();

	bool operator==(const Component& other);
	bool operator!=(const Component& other);

	virtual ~Component()
	{
		std::cout << "Component destructor!!!\n";
	}
};