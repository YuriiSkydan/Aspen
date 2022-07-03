#include "CircleCollider.h"
#include "imgui/imgui.h"

CircleCollider::CircleCollider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{}

void CircleCollider::UpdateGui()
{
	if (ImGui::CollapsingHeader("Circle Collider"))
	{
		int columnWidth = 110;
		int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Radius");
		ImGui::Spacing();

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Radius", &radius, 0.001f, 0.0f);

		ImGui::Columns(1);
	}
}