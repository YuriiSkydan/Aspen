#include "Camera.h"
#include "Transform.h"

void Camera::UpdateGui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		unsigned int columnWidth = 110;
		unsigned int itemSize = ImGui::GetWindowSize().x - columnWidth - 10;;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Background");
		ImGui::Spacing();
		ImGui::Text("Size"); ImGui::SameLine();

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, std::abs(ImGui::GetWindowSize().x - columnWidth));

		ImGui::SetNextItemWidth(itemSize);
		ImGui::ColorEdit4("##Background Color", (float*)&backgroundColor);
		ImGui::SetNextItemWidth(itemSize);
		ImGui::DragFloat("##Size", &size, 0.001f);

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

Matrix3x3f Camera::GetCameraMatrix() const
{
	Matrix3x3f cameraMatrix(1.0f);
	
	cameraMatrix = Rotate(cameraMatrix, -transform->angle);
	cameraMatrix = Scale(cameraMatrix, Vector2f(m_AspectRation, 1.0f) * (1.0f / size));
	cameraMatrix = Translate(cameraMatrix, -transform->position);
	
	//Matrix3x3f cameraMatrix(1.0f);
	//cameraMatrix = CameraMatrix(-transform->position, m_AspectRation);
	//cameraMatrix[0][0] *= 1.0f / m_Size;
	//cameraMatrix[1][1] *= 1.0f / m_Size;
	//cameraMatrix = Rotate(cameraMatrix, -transform->angle);

	//cameraMatrix = cameraMatrix.Transpose();

	return cameraMatrix;
}
