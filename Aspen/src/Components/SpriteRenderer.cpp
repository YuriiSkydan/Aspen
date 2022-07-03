#include <filesystem>

#include "SpriteRenderer.h"
#include "imgui/imgui.h"
#include "../GameObject/GameObject.h"

float square[]
{
	 0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f,  // top left
	 0.5f,  0.5f, 1.0f, 1.0f   // top right 
};

unsigned int indicies[]
{
	0, 1, 2,
	0, 3, 2 
};

void SpriteRenderer::UpdateGui()
{
	bool isOpen = ImGui::CollapsingHeader("##Sprite Renderer", ImGuiTreeNodeFlags_AllowItemOverlap);
	ImGui::SameLine();
	ImGui::Checkbox("##Is Enabled", &m_IsEnabled);
	ImGui::SameLine();
	ImGui::Text("Sprite Renderer");

	if (isOpen)
	{
		unsigned int columnWidth = 110;
		unsigned int itemWidth = (ImGui::GetWindowSize().x - columnWidth) - 20;

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		
		ImGui::Spacing();
		ImGui::Text("Order In Layer");
		ImGui::Spacing();
		ImGui::Text("Color");
		ImGui::Spacing();
		ImGui::Text("Texture");

		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, std::abs(ImGui::GetWindowSize().x - columnWidth));

		ImGui::SetNextItemWidth(itemWidth);
		ImGui::DragInt("##Order In Layer", &orderInLayer, 1, 0);

		ImGui::SetNextItemWidth(itemWidth);
		if (ImGui::ColorEdit4("##Color", (float*)&m_Color))
		{
			m_Shader.Bind();
			m_Shader.SetVec4f("spriteColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
		}

		ImGui::ImageButton((ImTextureID)m_Sprite.GetID(), { float(itemWidth), float(itemWidth) }, { 0, 1 }, { 1, 0 });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_PANEL_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = "Assets";
				texturePath /= path;

				std::wstring wPath = texturePath.c_str();
				std::string sPath(wPath.begin(), wPath.end());
				m_Sprite = Texture(sPath);
				//m_Sprite = Texture(path);
				//ImGui::GetDragDropPayload();
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Columns(1);
	}
}

SpriteRenderer::SpriteRenderer(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform), m_Shader("Shaders/StandartShader.vs", "Shaders/StandartShader.fs"),
	m_Sprite("Assets/Sprites/StandartSprite.png")
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	m_VertexBuffer.SetData(square, sizeof(square));
	m_IndexBuffer.SetData(indicies, 6);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	m_Shader.Bind();
	m_Shader.SetVec4f("spriteColor", m_Color.r, m_Color.g, m_Color.r, m_Color.a);
	m_Shader.SetInt("gameObjectID", gameObject->GetID());
}

void SpriteRenderer::SetColor(const Color& color)
{
	m_Color = color;
	m_Shader.Bind();
	m_Shader.SetVec4f("spriteColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);
}

void SpriteRenderer::SetSprite(const std::string_view path)
{
	// here's bug if you created denstructor in Texture
	Texture newSprite(path);
	m_Sprite = newSprite; 
}

void SpriteRenderer::Draw()
{
	m_Shader.Bind();
	m_Shader.SetMat3("transform", transform->GetTransformMatrix());
	m_Shader.SetVec4f("spriteColor", m_Color.r, m_Color.g, m_Color.b, m_Color.a);

	m_Sprite.Bind(0);
	m_Shader.SetInt("sprite", 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}