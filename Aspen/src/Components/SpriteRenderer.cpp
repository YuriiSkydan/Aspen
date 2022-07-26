#include <filesystem>

#include "SpriteRenderer.h"
#include "../GameObject/GameObject.h"
#include "../Input/Input.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
	m_Sprite = TextureLibrary::Get()->GetTexture("Assets/Sprites/StandartSprite.png");
	m_Shader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");
}

void SpriteRenderer::SetColor(const Color& color)
{
	m_Color = color;
}

void SpriteRenderer::SetSprite(const std::string_view path)
{
	m_Sprite = TextureLibrary::Get()->GetTexture(path.data());
}

void SpriteRenderer::SetSprite(const std::shared_ptr<Texture>& sprite)
{
	m_Sprite = sprite;
}