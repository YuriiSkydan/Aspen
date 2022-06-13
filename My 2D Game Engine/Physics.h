// Old Physics

//#pragma once
//
//#ifndef PHYSICS_H
//#define PHYSICS_H
//
//#include "src/Components/Transform.h"
//#include <vector>
//#include "src/Log/Log.h"
//#include <string> // delete later
//
////class Collider : public Component
////{
////	virtual bool TestCollision(Collider* other) = 0;
////};
//
//struct Material
//{
//	float dencity;
//	float restitution = 0.5f;
//};
//
//struct MassData
//{
//	float mass = 1.0f;
//	float invMass = 1.0f;
//
//	float inertia = 1.0f;
//	float invInertia = 1.0f;
//};
//
//
//class BoxCollider;
//class Rigidbody;
//
//bool OverlapOnAxis(BoxCollider* box1, BoxCollider* box2, Vector2f axis);
//
//Vector2f GetInterval(BoxCollider* box, Vector2f axis);
//
//Vector2f GetInterval(BoxCollider* box1, BoxCollider* box2, Vector2f axis);
//
//void PositionalCorrection(Rigidbody* objA, Rigidbody* bojB);
//
//float FindAxisLeastPenetration(int& faceIndex, BoxCollider* A, BoxCollider* B);
//
//void FindIncidentFace(Vector2f* v, BoxCollider* RefBox, BoxCollider* IncBox, int referenceIndex);
//
//int Clip(Vector2f n, float c, Vector2f* face);
//
//struct Manifold
//{
//	BoxCollider* A;
//	BoxCollider* B;
//
//	Rigidbody* rigidbodyA;
//	Rigidbody* rigidbodyB;
//
//	float penetration;
//	Vector2f normal;
//	Vector2f contacts[2];
//	int contactCount = 0;
//
//	float restitution = 0.0f;
//	float dynamicFriction = 0.0f;
//	float staticFriction = 0.0f;
//
//	Manifold(BoxCollider* a, BoxCollider* b);
//
//	void Solve();
//	void Initialize();
//	void ApplyImpulse();
//	void PositionCorrection();
//	void InfinitiMassCorrection();
//};
//
//class BoxCollider // : public Collider
//{
//public:
//	Transform* transform;
//	Vector2f m_Min;
//
//	Vector2f m_Verticies[4];
//	Vector2f m_Normals[4];
//
//	Material material;
//public:
//	BoxCollider(Transform* transform) : transform(transform)
//	{
//		Matrix3x3f transformMatrix = transform->GetTransform();
//		m_Verticies[0] = transformMatrix * Vector2f(-0.5f, -0.5f) + transform->position;
//		m_Verticies[1] = transformMatrix * Vector2f( 0.5f, -0.5f) + transform->position;
//		m_Verticies[2] = transformMatrix * Vector2f( 0.5f,  0.5f) + transform->position;
//		m_Verticies[3] = transformMatrix * Vector2f(-0.5f,  0.5f) + transform->position;
//
//		m_Normals[0] = Vector2f( 0.0f, -1.0f);
//		m_Normals[1] = Vector2f( 1.0f,  0.0f);
//		m_Normals[2] = Vector2f( 0.0f,  1.0f);
//		m_Normals[3] = Vector2f(-1.0f,  0.0f);
//	}
//
//	void Update()
//	{
//	}
//
//	Vector2f GetSupport(const Vector2f& axis)
//	{
//		float bestProjection = -FLT_MAX;
//		Vector2f bestVertex;
//
//		for (size_t i = 0; i < 4; i++)
//		{
//			Vector2f v = m_Verticies[i];
//			float projection = Dot(v, axis);
//
//			if (projection > bestProjection)
//			{
//				bestProjection = projection;
//				bestVertex = v;
//			}
//		}
//
//		return bestVertex;
//	}
//
//	bool HasCollided(BoxCollider* other)
//	{
//		/*	Vector2f axesToTest[4] =
//			{ transform->Right(), transform->Up(),
//			 other->transform->Right(), other->transform->Up() };
//
//			for (size_t i = 0; i < 4; i++)
//			{
//				if (!OverlapOnAxis(this, other, axesToTest[i].Normalized()))
//					return false;
//			}*/
//
//			//Matrix3x3f axes = transform->GetTransform();
//
//			//Vector2f axesToTest[2] =
//			//{ Vector2f(axes[0][0], axes[1][0]), Vector2f(axes[0][1], axes[1][1]) };
//
//			//for (size_t i = 0; i < 2; i++)
//			//{
//			//	if (!OverlapOnAxis(this, other, axesToTest[i].Normalized()))
//			//		return false;
//			//}
//
//		/*	if (m_Max.x < other->m_Min.x || m_Min.x > other->m_Max.x) return false;
//			if (m_Max.y < other->m_Min.y || m_Min.y > other->m_Max.y) return false;*/
//
//		return true;
//	}
//};
//
//
//
//
////class CircleCollider : public Collider
////{
////public:
////	float m_Radius;
////
////public:
////	//CircleCollider() {} 
////};
//
//
////class CollisionObject
////{
////protected:
////	Collider* m_Collider;
////};
//
//
//class Rigidbody : public Component
//{
//public:
//	friend class PhysicsWorld;
//
//	Vector2f m_Force;
//	MassData m_MassData;
//
//	std::shared_ptr<BoxCollider> m_Collider;
//
//public:
//	Vector2f velocity;
//
//	float angularVelocity = 0;
//	float torque = 0;
//
//	float gravityScale;
//
//	std::string name = "GameObject";
//	static inline size_t count = 0;
//
//private:
//	void UpdateGui() override
//	{
//		if (ImGui::CollapsingHeader("Rigidbody"))
//		{
//			if (ImGui::DragFloat("Mass", &m_MassData.mass, 0.01f, 0.0f))
//				m_MassData.invMass = (m_MassData.mass != 0) ? (1 / m_MassData.mass) : (0.0f);
//			ImGui::DragFloat2("Velocity", (float*)&velocity);
//		}
//	}
//
//public:
//	Rigidbody(GameObject* gameObject, Transform* transform)
//		:Component(gameObject, transform)
//	{
//		m_Collider = std::make_shared<BoxCollider>(transform);
//		name += std::to_string(count++);
//	}
//
//	void Update() override
//	{
//		m_Collider->Update();
//	}
//
//	void ApplyImpulse(Vector2f impulse, Vector2f angular)
//	{
//		velocity += m_MassData.invMass * impulse;
//		angularVelocity += m_MassData.invInertia * Cross(angular, impulse);
//	}
//
//	void SetMass(float mass) { m_MassData.mass = mass; }
//	float GetMass() const { return m_MassData.mass; }
//
//	//void AddForce();
//
//};
//
//class PhysicsWorld
//{
//private:
//	std::vector<Rigidbody*> m_Objects;
//	Vector2f m_Gravity = Vector2f(0.0f, -9.81f);
//
//public:
//	PhysicsWorld() = default;
//
//	void AddObject(Rigidbody* object)
//	{
//		m_Objects.push_back(object);
//	}
//	void RemoveObject(Rigidbody* object) {}
//
//	void Update(float dt);
//};
//
//#endif

#include "src/Components/Transform.h"
#include "src/Math/Math.h"
#include "box2d/b2_body.h"

struct Material
{
	float friction = 0.4f;
	float restitution = 0.0f;
};

class BoxCollider : public Component
{
public:
	Vector2f offset = { 0.0f, 0.0f };
	Vector2f size = { 0.5f, 0.5f };

	Material material;

public:
	BoxCollider(GameObject* gameObject, Transform* transform)
		: Component(gameObject, transform)
	{
	}

	void UpdateGui() override
	{
		if (ImGui::CollapsingHeader("Box Collider"))
		{
			int columnWidth = 110;
			int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columnWidth);

			ImGui::Spacing();
			ImGui::Text("Size");
			ImGui::Spacing();
			ImGui::Text("Offset");
			ImGui::Spacing();
			ImGui::Text("Material");
			ImGui::Spacing();
			ImGui::Text("Friction");
			ImGui::Spacing();

			ImGui::NextColumn();
			ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat2("##Size", (float*)&size, 0.01f, 0.0f, FLT_MAX, "%.3f");
			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat2("##Offset", (float*)&offset, 0.1f);
			ImGui::NewLine();
			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat("##Friction", &material.friction, 0.01f, 0.0f, FLT_MAX);
			//ImGui::Text("Bounciness     ");
			//ImGui::SameLine();
			//ImGui::DragFloat("##Bounciness", &material.restitution, 0.01f, 0.0f, FLT_MAX);
			ImGui::Columns(1);
		}
	}
};

class CircleCollider : public Component
{
public:
	float radius = 0.5f;

public:
	CircleCollider(GameObject* gameObject, Transform* transform)
		: Component(gameObject, transform)
	{

	}

	void UpdateGui() override
	{
		if (ImGui::CollapsingHeader("Circle Collider"))
		{
			int columnWidth = 110;
			int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columnWidth);

			ImGui::Spacing();
			ImGui::Text("Radius");
			ImGui::Spacing();

			ImGui::NextColumn();
			ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat("##Radius", &radius, 0.001f, 0.0f);

			ImGui::Columns(1);
		}
	}
};

class Rigidbody : public Component
{
private:
	b2BodyType type = b2_dynamicBody;
	float m_Mass;
	float m_LinearDrag;
	float m_AngularDrag;

public:
	b2Body* body = nullptr;
	Material material;

	float gravityScale;

public:
	Rigidbody(GameObject* gameObject, Transform* transform) :
		Component(gameObject, transform)
	{
	}

	void Update() override
	{
		transform->position = (float*)&body->GetPosition();
		transform->angle = -ToDegrees(body->GetAngle());
	}

	void UpdateGui() override
	{
		b2BodyType type;
		type = b2_dynamicBody;
		if (ImGui::CollapsingHeader("Rigidbody"))
		{
			int columnWidth = 110;
			int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columnWidth);

			ImGui::Spacing();
			ImGui::Text("Body Type");
			ImGui::Spacing();
			ImGui::Text("Material");
			ImGui::Spacing();
			ImGui::Text("Friction");
			ImGui::Spacing();
			ImGui::Text("Bounciness");

			ImGui::NextColumn();
			ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

			if (ImGui::BeginCombo("##BodyType", "Dynamic"))
			{
				//if(ImGui::Combo("Static", "")
				ImGui::EndCombo();
			}

			ImGui::NewLine();
			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat("##Friction", &material.friction, 0.01f, 0.0f, FLT_MAX);
			ImGui::SetNextItemWidth(itemWidth);
			ImGui::DragFloat("##Bounciness", &material.restitution, 0.01f, 0.0f, FLT_MAX);
		
			ImGui::Columns(1);
		}
	}

	void SetMass(float mass)
	{
		if (mass > 0)
			m_Mass = mass;
	}
	float GetMass() const { return m_Mass; }
};