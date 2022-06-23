#pragma once

#ifndef SCRIPT
#include "Component.h"
#endif

#include "../Math/MatrixTransform.h"

enum class Space { Self, World };

class Transform : public Component
{
public:
	Vector2f position;
	Vector2f scale;
	float angle;

	Transform* parent;
	Transform* child;
private:
	void UpdateGui() override;

public:
	Transform(GameObject* gameObject);
	Transform(GameObject* gameObject, Transform* transform);

	void Awake() override {}
	void Start() override {}
	void Update() override {}
	void FixedUpdate() override {}
	void LateUpdate() override {}
	void OnDestroy() override {}

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