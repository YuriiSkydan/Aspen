#pragma once
#include "Component.h"
#include "../Math/MatrixTransform.h"

enum class ASPEN Space { Self, World };

class ASPEN Transform : public Component
{
public:
	Vector2f position;
	Vector2f scale;
	float angle;

	//Transform* parent;
	//Transform* child;

public:
	Transform(GameObject* gameObject);
	Transform(GameObject* gameObject, Transform* transform);

	Matrix3x3f GetTransformMatrix() const;
	Vector2f Right() const;
	Vector2f Up() const;

	void Translate(const Vector2f& translation, Space relativeTo = Space::Self);
	void Rotate(float angle, Space relativeTo = Space::Self);
	//void LookAt();

	bool operator==(const Transform* other);
	bool operator!=(const Transform* other);

	~Transform() = default;
};