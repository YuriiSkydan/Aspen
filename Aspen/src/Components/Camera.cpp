#include "Camera.h"
#include "Transform.h"

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
