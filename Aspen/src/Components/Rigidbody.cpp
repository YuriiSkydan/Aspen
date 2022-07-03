#include "imgui/imgui.h"
#include "../Math/Math.h"
#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
}

void Rigidbody::UpdateGui()
{
	if (ImGui::CollapsingHeader("Rigidbody"))
	{
		int columnWidth = 110;
		int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Body Type");
		ImGui::Spacing();
		ImGui::Text("Mass");
		ImGui::Spacing();
		ImGui::Text("Gravity Scale");
		ImGui::Spacing();
		ImGui::Text("Linear  Drag");
		ImGui::Spacing();
		ImGui::Text("Angular Drag");

		ImGui::NewLine();
		ImGui::Spacing();

		ImGui::Text("Fixed Rotation");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

		std::string bodyTypeStr;
		if (type == BodyType(b2_dynamicBody))
			bodyTypeStr = "Dynamic";
		else if (type == BodyType(b2_staticBody))
			bodyTypeStr = "Static";
		else
			bodyTypeStr = "Kinematic";

		ImGui::SetNextItemWidth(itemWidth);
		if (ImGui::BeginCombo("##BodyType", bodyTypeStr.c_str()))
		{
			if (ImGui::Selectable("Dynamic"))
				type = BodyType(b2_dynamicBody);
			if (ImGui::Selectable("Static"))
				type = BodyType(b2_staticBody);
			if (ImGui::Selectable("Kinematic"))
				type = BodyType(b2_kinematicBody);

			ImGui::EndCombo();
		}

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Mass", &m_Mass, 0.01f);

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##GravityScale", &gravityScale, 0.01f);

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##LinearDrag", &m_LinearDrag, 0.01f);

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##AngularDrag", &m_AngularDrag, 0.01f);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("##FixedRotation", &fixedRotation);
		ImGui::Columns(1);
	}
}

void Rigidbody::Update()
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

void Rigidbody::FixedUpdate()
{
	transform->position = (float*)&m_Body->GetPosition();
	transform->angle = -ToDegrees(m_Body->GetAngle());
}

void Rigidbody::AddForce(const Vector2f& force, const ForceMode& mode)
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

void Rigidbody::AddTorque(float torque)
{
	m_Body->ApplyTorque(torque, true);
}

void Rigidbody::SetLinearVelocity(const Vector2f& velocity)
{
	b2Vec2 b2Velocity(velocity.x, velocity.y);
	m_Body->SetLinearVelocity(b2Velocity);
}

void Rigidbody::SetAngularVelocity(float velocity)
{
	m_Body->SetAngularVelocity(velocity);
}

void Rigidbody::SetMass(float mass)
{
	if (mass > 0)
		m_Mass = mass;
}

void Rigidbody::SetLinearDrag(float drag)
{
	if (drag >= 0)
		m_LinearDrag = drag;
}

void Rigidbody::SetAngularDrag(float drag)
{
	m_AngularDrag = drag;
}

void Rigidbody::SetBody(b2Body* body)
{
	m_Body = body;
}
