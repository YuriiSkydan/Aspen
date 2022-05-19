#include "Transform.h"

void Transform::UpdateGui() // optimize this function
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position       "); 
		ImGui::SameLine();
		ImGui::SetNextItemWidth(165);
		ImGui::DragFloat2("##Position ", (float*)&position, 0.02f);

		//if (ImGui::DragFloat2("##Position", (float*)&position, 0.01f))
		//{
		//	//transform = Translate(transform, position);
		//}
		ImGui::Text("Rotation       ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(165);
		ImGui::DragFloat("##Rotation", &angle, 0.02f);

		ImGui::Text("Scale          ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(165);
		ImGui::DragFloat2("##Scale  ", (float*)&scale.x, 0.01f);
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