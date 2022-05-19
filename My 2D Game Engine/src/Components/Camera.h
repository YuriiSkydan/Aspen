#pragma once

#include "Component.h"
#include "../Math/MatrixTransform.h"
#include "../Components/SpriteRenderer.h"

class Camera : public Component
{
private:
	Color m_Background;

	float m_Size = 1;
	float m_AspectRation;

	unsigned int m_Width = 0, m_Heigth = 0;
private:
	void UpdateGui() override;

public:
	Camera(GameObject* gameObject, Transform* transform);

	void SetRatio(float aspectRation);
	Matrix3x3f GetCameraMatrix();

	Color GetBackgroundColor() const { return m_Background; }
};

