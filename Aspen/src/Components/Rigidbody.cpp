#include "Rigidbody.h"
#include "../Math/Math.h"

Rigidbody::Rigidbody(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
}

void Rigidbody::Update()
{
	if (m_Body != nullptr)
	{
		b2Vec2 bodyPosition = m_Body->GetPosition();
		Vector2f position = transform->position;

		float bodyAngle = m_Body->GetAngle();
		float angle = transform->angle;

		if (position.x != bodyPosition.x || position.y != bodyPosition.y ||
			angle != -ToDegrees(bodyAngle))
		{
			bodyPosition = b2Vec2(position.x, position.y);

			m_Body->SetTransform(bodyPosition, ToRads(-angle));
			m_Body->SetAwake(true);
		}
	}
}

void Rigidbody::FixedUpdate()
{
	if (m_Body != nullptr)
	{
		transform->position = (float*)&m_Body->GetPosition();
		transform->angle = -ToDegrees(m_Body->GetAngle());
	}
}

void Rigidbody::AddForce(const Vector2f& force, const ForceMode& mode)
{
	if (m_Body != nullptr)
	{
		b2Vec2 b2Force(force.x, force.y);

		switch (mode)
		{
		case ForceMode::Force:
			m_Body->ApplyForceToCenter(b2Force, true);
			break;

		case ForceMode::Impulse:
			m_Body->ApplyLinearImpulseToCenter(b2Force, true);
			break;
		}
	}
}

void Rigidbody::AddAngularImpulse(float impulse)
{
	if (m_Body != nullptr)
		m_Body->ApplyAngularImpulse(impulse, true);
}

void Rigidbody::AddTorque(float torque)
{
	if (m_Body != nullptr)
		m_Body->ApplyTorque(torque, true);
}

void Rigidbody::SetLinearVelocity(const Vector2f& velocity)
{
	if (m_Body != nullptr)
	{
		b2Vec2 b2Velocity(velocity.x, velocity.y);
		m_Body->SetLinearVelocity(b2Velocity);
	}
}

void Rigidbody::SetAngularVelocity(float velocity)
{
	if (m_Body != nullptr)
		m_Body->SetAngularVelocity(velocity);
}

void Rigidbody::SetMass(float mass)
{
	if (mass > 0)
	{
		m_Mass = mass;

		if (m_Body != nullptr)
		{
			b2MassData data = m_Body->GetMassData();
			data.mass = mass;
			m_Body->SetMassData(&data);
		}
	}
}

void Rigidbody::SetLinearDrag(float drag)
{
	if (drag >= 0)
	{
		m_LinearDrag = drag;

		if (m_Body != nullptr)
		{
			m_Body->SetLinearDamping(drag);
		}
	}
}

void Rigidbody::SetFixedRotation(bool value)
{
	m_FixedRotation = value;

	if (m_Body != nullptr)
		m_Body->SetFixedRotation(m_FixedRotation);
}

void Rigidbody::SetGravityScale(float scale)
{
	m_GravityScale = scale;

	if (m_Body != nullptr)
		m_Body->SetGravityScale(scale);
}

void Rigidbody::SetBodyType(BodyType type)
{
	m_Type = type;

	if (m_Body != nullptr)
		m_Body->SetType(b2BodyType(type));
}

void Rigidbody::SetAngularDrag(float drag)
{
	if (drag >= 0)
	{
		m_AngularDrag = drag;

		if (m_Body != nullptr)
		{
			m_Body->SetAngularDamping(drag);
		}
	}
}

Vector2f Rigidbody::GetLinearVelocity() const
{
	auto velocity = m_Body->GetLinearVelocity();
	return Vector2f(velocity.x, velocity.y);
}

void Rigidbody::SetBody(b2Body* body)
{
	m_Body = body;
}

void Rigidbody::Serialize(json& out) const
{
	out["Rigidbody"] =
	{
		{ "Mass", m_Mass },
		{ "LinearDrag", m_LinearDrag },
		{ "AngularDrag", m_AngularDrag },
		{ "BodyType", int(m_Type) },
		{ "GravityScale", m_GravityScale},
		{ "FixedRotation", m_FixedRotation }
	};
}

void Rigidbody::Deserialize(json& in)
{
	SetMass(in["Mass"]);
	SetLinearDrag(in["LinearDrag"]);
	SetAngularDrag(in["AngularDrag"]);
	SetBodyType(BodyType(in["BodyType"]));
	SetGravityScale(in["GravityScale"]);
	SetFixedRotation(in["FixedRotation"]);
}