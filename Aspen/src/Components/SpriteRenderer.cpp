#include <filesystem>

#include "SpriteRenderer.h"
#include "../GameObject/GameObject.h"
#include "../Input/Input.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
	//glGenVertexArrays(1, &m_VAO);
	//glBindVertexArray(m_VAO);

	//m_VertexBuffer.SetData(square, sizeof(square));
	//m_IndexBuffer.SetData(indicies, 6);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	m_Sprite = TextureLibrary::Get()->GetTexture("Assets/Sprites/StandartSprite.png");
	m_Shader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");
}

void SpriteRenderer::SetColor(const Color& color)
{
	m_Color = color;
}

void SpriteRenderer::SetSprite(const std::string_view path)
{
	// here's bug if you created denstructor in Texture
	
	m_Sprite = TextureLibrary::Get()->GetTexture(path.data());
}

void SpriteRenderer::SetSprite(const std::shared_ptr<Texture>& sprite)
{
	m_Sprite = sprite;
}