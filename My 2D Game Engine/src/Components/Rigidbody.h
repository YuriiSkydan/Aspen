#pragma once
#include "Transform.h"
#include "Material.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../Math/Math.h"
#include "../box2d/b2_body.h"

enum class BodyType { Static = 0, Kinematic, Dynamic };

class Rigidbody : public Component
{
private:
	b2Body* m_Body = nullptr;

	float m_Mass = 1.0f;
	float m_LinearDrag = 0.0f;
	float m_AngularDrag = 0.05f;

	friend class Scene;
public:
	BodyType type = BodyType::Dynamic;
	float gravityScale = 1.0f;
	bool fixedRotation = false;

private:
	void UpdateGui() override;
	void SetBody(b2Body* body);

public:
	Rigidbody(GameObject* gameObject, Transform* transform);

	void Update() override;

	void SetMass(float mass);
	void SetLinearDrag(float drag);
	void SetAngularDrag(float drag);

	float GetLinearDrag() const { return m_LinearDrag; }
	float GetAngularDrag() const { return m_AngularDrag; }
	float GetMass() const { return m_Mass; }
};