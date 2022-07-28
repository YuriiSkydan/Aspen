#pragma once
#include "Transform.h"
#include "box2d/b2_body.h"

enum class ASPEN BodyType { Static = 0, Kinematic, Dynamic };
enum class ASPEN ForceMode { Force = 0, Acceleration, Impulse, VelocityChange };

class ASPEN Rigidbody : public Component
{
private:
	b2Body* m_Body = nullptr;

	float m_Mass = 1.0f;
	float m_LinearDrag = 0.0f;
	float m_AngularDrag = 0.05f;

	friend class Scene;
	friend class Inspector;
public:
	BodyType type = BodyType::Dynamic;
	float gravityScale = 1.0f;
	bool fixedRotation = false;

private:
	void SetBody(b2Body* body);

public:
	Rigidbody(GameObject* gameObject, Transform* transform);

	void Update() override;
	void FixedUpdate() override;

	void AddForce(const Vector2f& force, const ForceMode& mode = ForceMode::Force);
	void AddTorque(float torque);
	void SetLinearVelocity(const Vector2f& velocity);
	void SetAngularVelocity(float velocity);

	void SetMass(float mass);
	void SetLinearDrag(float drag);
	void SetAngularDrag(float drag);

	Vector2f GetLinearVelocity() const;
	float GetAngularVelocity() const { return m_Body->GetAngularVelocity(); };

	float GetLinearDrag() const { return m_LinearDrag; }
	float GetAngularDrag() const { return m_AngularDrag; }
	float GetMass() const { return m_Mass; }
};