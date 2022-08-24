#pragma once
#include <vector>

#include "Component.h"
#include "../Math/MatrixTransform.h"

enum class ASPEN Space { Self, World };

class ASPEN Transform : public Component
{
private:
	Transform* m_Parent = nullptr;
	std::vector<Transform*> m_Childs;

public:
	Vector2f position;
	Vector2f scale;
	float angle;

	//Need to work on local properties
	Vector2f localPosition;
	Vector2f localScale;
	float localAngle;

public:
	Transform(GameObject* gameObject);
	Transform(GameObject* gameObject, Transform* transform);

	void Reset() override;

	void Translate(const Vector2f& translation, Space relativeTo = Space::Self);
	void Rotate(float angle, Space relativeTo = Space::Self);
	//void LookAt();

	void AddChild(GameObject* child);
	void AddChild(Transform* child);
	void SetParent(Transform* parent);

	const std::vector<Transform*>& GetChilds() const { return m_Childs; }
	Transform* GetParent() const { return m_Parent; }
	Matrix3x3f GetTransformMatrix() const;
	Vector2f Right() const;
	Vector2f Up() const;

	bool operator==(const Transform* other);
	bool operator!=(const Transform* other);

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

	~Transform() = default;
};