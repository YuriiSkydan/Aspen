#include "Camera.h"
#include "Transform.h"

void Camera::UpdateGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		int columnWidth = 110;
		float itemSize = ImGui::GetWindowSize().x - columnWidth - 10;;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Background");
		ImGui::Spacing();
		ImGui::Text("Size"); ImGui::SameLine();

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

		ImGui::SetNextItemWidth(itemSize);
		ImGui::ColorEdit4("##Background Color", (float*)&m_Background);
		ImGui::SetNextItemWidth(itemSize);
		ImGui::DragFloat("##Size", &m_Size, 0.001f);

		ImGui::Columns(1);
	}
}

Camera::Camera(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
}

void Camera::SetRatio(float aspectRation)
{
	m_AspectRation = aspectRation;
}

Matrix3x3f Camera::GetCameraMatrix()
{
	Matrix3x3f cameraMatrix(1.0f);
	cameraMatrix[0][0] = m_AspectRation;
	cameraMatrix = Rotate(cameraMatrix, transform->angle);
	cameraMatrix = Translate(cameraMatrix, -transform->position);


	//Matrix3x3f cameraMatrix(1.0f);
	//cameraMatrix = CameraMatrix(-transform->position, m_AspectRation);
	//cameraMatrix[0][0] *= 1.0f / m_Size;
	//cameraMatrix[1][1] *= 1.0f / m_Size;
	//cameraMatrix = Rotate(cameraMatrix, -transform->angle);

	//cameraMatrix = cameraMatrix.Transpose();

	return cameraMatrix;
}
