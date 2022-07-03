#include "BoxCollider.h"
#include "imgui/imgui.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform)
	: Trigger(gameObject, transform)
	, Collision(gameObject, transform)
	, Component(gameObject, transform)
{
}

void BoxCollider::UpdateGui()
{
	if (ImGui::CollapsingHeader("Box Collider"))
	{
		int columnWidth = 110;
		int itemWidth = ImGui::GetWindowSize().x - columnWidth - 10;
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Spacing();
		ImGui::Text("Size");
		ImGui::Spacing();
		ImGui::Text("Offset");
		ImGui::Spacing();
		ImGui::Text("Material");
		ImGui::Spacing();
		ImGui::Text("Friction");
		ImGui::Spacing();

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x - columnWidth);

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat2("##Size", (float*)&size, 0.01f, 0.0f, FLT_MAX, "%.3f");
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat2("##Offset", (float*)&offset, 0.1f);
		ImGui::NewLine();
		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragFloat("##Friction", &material.friction, 0.01f, 0.0f, FLT_MAX);
		//ImGui::Text("Bounciness     ");
		//ImGui::SameLine();
		//ImGui::DragFloat("##Bounciness", &material.restitution, 0.01f, 0.0f, FLT_MAX);
		ImGui::Columns(1);
	}
}
