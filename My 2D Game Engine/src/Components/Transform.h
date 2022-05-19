#pragma once

#include "Component.h"
#include "../Math/MatrixTransform.h"

#include <iostream> // don't forget to delete

class Transform : public Component
{
public:
	Vector2f position;
	Vector2f scale;

	float angle;

private:
	void UpdateGui() override;

public:
	Transform(GameObject* gameObject);

	//void Translate(Vector2<float> translation);
	//void Rotate();

	//void SetParent(Transform* parent);
	//void SetChild(Transform* child);
	
	//void GetParent() const;
	//void GetChild() const;

	Matrix3x3f GetTransform() const;
	Vector2f Right() const;
	Vector2f Up() const;

	//bool operator==(const Transform* transform);
	//bool operator!=(const Transform* transform);

	~Transform() = default;
};