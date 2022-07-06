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