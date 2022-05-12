#include "Camera.h"
#include "Transform.h"

void Camera::UpdateGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Background");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Background Color", (float*)&m_Background);

		ImGui::Text("Size"); ImGui::SameLine();
		ImGui::DragFloat("##Size", &m_Size, 0.001f);
	}
}

Camera::Camera(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
}

void Camera::SetRatio(float ratio)
{
	m_Ratio = ratio;
}

Matrix3x3f Camera::GetCameraMatrix()
{
	Matrix3x3f cameraMatrix;
	cameraMatrix = CameraMatrix(-transform->position, m_Ratio);
	cameraMatrix[0][0] *= 1.0f / m_Size;
	cameraMatrix[1][1] *= 1.0f / m_Size;
	cameraMatrix = Rotate(cameraMatrix, -transform->angle);

	return cameraMatrix;
}
