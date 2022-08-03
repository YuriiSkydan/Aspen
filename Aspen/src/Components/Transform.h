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

public:
	Transform(GameObject* gameObject);
	Transform(GameObject* gameObject, Transform* transform);

	Matrix3x3f GetTransformMatrix() const;
	Vector2f Right() const;
	Vector2f Up() const;

	void Translate(const Vector2f& translation, Space relativeTo = Space::Self);
	void Rotate(float angle, Space relativeTo = Space::Self);
	//void LookAt();

	void AddChild(GameObject* child);
	void AddChild(Transform* child);
	void SetParent(Transform* parent);
	Transform* GetParent() const { return m_Parent; }

	const std::vector<Transform*>& GetChilds() const { return m_Childs; }

	bool operator==(const Transform* other);
	bool operator!=(const Transform* other);

	~Transform() = default;
};