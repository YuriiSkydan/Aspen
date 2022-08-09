#pragma once
#include <iostream> // delete later
#include "JSON/json.hpp"
#include "../Core/Core.h"

class Transform;
class GameObject;

using namespace nlohmann;

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
	T* GetComponent() const;

	template<typename T>
	bool HasComponent() const;

	bool operator==(const Component& other);
	bool operator!=(const Component& other);

	virtual void Serialize(json& out) const;
	virtual void Deserialize(json& in);

	virtual ~Component()
	{
		std::cout << "Component destructor!!!\n";
	}
};