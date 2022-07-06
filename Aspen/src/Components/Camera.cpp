#include "Camera.h"
#include "Transform.h"

Camera::Camera(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform)
{
}

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
	
	//Matrix3x3f cameraMatrix(1.0f);
	//cameraMatrix = CameraMatrix(-transform->position, m_AspectRation);
	//cameraMatrix[0][0] *= 1.0f / m_Size;
	//cameraMatrix[1][1] *= 1.0f / m_Size;
	//cameraMatrix = Rotate(cameraMatrix, -transform->angle);

	//cameraMatrix = cameraMatrix.Transpose();

	return cameraMatrix;
}
