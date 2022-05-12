#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream> // for debug

class Transform;
class GameObject;

class Component
{
public:
	GameObject* gameObject;
	Transform* transform;

	bool m_IsEnabled = true;

	friend class GameObject;
	friend class Inspector;
private:
	virtual void UpdateGui() {};

public:
	Component(GameObject* gameObject, Transform* transform);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual void LateUpdate() {}
	virtual void OnDestroy() {}
	virtual bool IsEnabled() const { return m_IsEnabled; }

	~Component()
	{
		std::cout << "Component destructor!!!\n";
	}
};

#endif