#include "Transform.h"

void Transform::UpdateGui() // optimize this function
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		int columnWidth = 110;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Position");
		ImGui::Spacing();
		ImGui::Text("Rotation");
		ImGui::Spacing();
		ImGui::Text("Scale");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);
		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - columnWidth - 10);
		ImGui::DragFloat2("##Position ", (float*)&position, 0.02f);
		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - columnWidth - 10);
		ImGui::DragFloat("##Rotation", &angle, 0.02f);
		ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - columnWidth - 10);
		ImGui::DragFloat2("##Scale  ", (float*)&scale.x, 0.01f);

		ImGui::Columns(1);
	}
}

Transform::Transform(GameObject* gameObject)
	:Component(gameObject, this), position{0.0f, 0.0f},
	 scale{ 1.0f, 1.0f }, angle(0)
{

}

Matrix3x3f Transform::GetTransform() const
{
	Matrix3x3f transform = Matrix3x3f(1.0f);

	transform = Translate(transform, position);
	transform = Rotate(transform, angle);
	transform = Scale(transform, scale);

	return transform;
}

Vector2f Transform::Right() const
{
	Matrix3x3f trans = GetTransform();
	Vector2f right(trans[0][0], trans[1][0]);
	right.Normalize();
	return right;
}

Vector2f Transform::Up() const
{
	Matrix3x3f trans = GetTransform();
	Vector2f up(trans[0][1], trans[1][1]);
	up.Normalize();
	return up;
}