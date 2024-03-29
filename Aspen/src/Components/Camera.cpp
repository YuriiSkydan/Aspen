#include "Camera.h"

Camera::Camera(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{ }

void Camera::SetRatio(float aspectRation)
{
	m_AspectRation = aspectRation;
}

Matrix3x3f Camera::GetCameraMatrix() const
{
	Matrix3x3f cameraMatrix(1.0f);

	cameraMatrix = MatrixTransform::Rotate(cameraMatrix, -transform->angle);
	cameraMatrix = MatrixTransform::Scale(cameraMatrix, Vector2f(m_AspectRation, 1.0f) * (1.0f / size));
	cameraMatrix = MatrixTransform::Translate(cameraMatrix, -transform->position);

	return cameraMatrix;
}

void Camera::Serialize(json& out) const
{
	out["Camera"] =
	{
		{ "Size", size},
		{ "Aspect ration",  GetAspectRation()},
		{ "Background color",
		{ { "R", backgroundColor.r},
		  { "G", backgroundColor.g},
		  { "B", backgroundColor.b},
		  { "A", backgroundColor.a} }}
	};

	Component::Serialize(out["Camera"]);
}

void Camera::Deserialize(json& in)
{
	size = in["Size"];
	SetRatio(in["Aspect ration"]);

	Color color;
	color.r = in["Background color"]["R"];
	color.g = in["Background color"]["G"];
	color.b = in["Background color"]["B"];
	color.a = in["Background color"]["A"];
	backgroundColor = color;

	Component::Deserialize(in);
}