#include "Transform.h"

void Transform::UpdateGui() // optimize this function
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		unsigned int columnWidth = 110;
		unsigned int itemWidth = (ImGui::GetWindowSize().x - columnWidth) - 10;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Position");
		ImGui::Spacing();
		ImGui::Text("Rotation");
		ImGui::Spacing();
		ImGui::Text("Scale");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, std::abs(ImGui::GetWindowSize().x - columnWidth));
		
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat2("##Position ", (float*)&position, 0.02f);
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Rotation", &angle, 0.02f);
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat2("##Scale  ", (float*)&scale.x, 0.01f);

		ImGui::Columns(1);
	}
}

Transform::Transform(GameObject* gameObject)
	:Component(gameObject, this), position{0.0f, 0.0f},
	 scale{ 1.0f, 1.0f }, angle(0)
{

}

Transform::Transform(GameObject* gameObject, Transform* transform)
	:Component(gameObject, this) 
{
}

Matrix3x3f Transform::GetTransform() const
{
	Matrix3x3f transform = Matrix3x3f(1.0f);

	transform = Translate(transform, position);
	transform = Scale(transform, scale);
	transform = Rotate(transform, angle);

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