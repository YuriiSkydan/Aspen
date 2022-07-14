#pragma once
#include <iostream> // for debug

#include "../Core/Core.h"

class Transform;
class GameObject;

class ASPEN Component
{
private:
	bool m_IsEnabled = true;

public:
	GameObject* gameObject;
	Transform* transform;

public:
	Component(GameObject* gameObject, Transform* transform);
	Component(const Component& other) = delete;
	const Component& operator=(const Component& other);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual void OnDestroy() {}

	void SetEnabled(bool enabled);
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