#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject, Transform* transform)
	:Component(gameObject, transform)
{
	TextureLibrary::Get()->GetTexture("Assets/Sprites/StandartSprite.png", m_Sprite);
	m_Shader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");
}

void SpriteRenderer::SetColor(const Color& color)
{
	m_Color = color;
}

void SpriteRenderer::SetSprite(const std::string_view path)
{
	TextureLibrary::Get()->GetTexture(path.data(), m_Sprite);
}

void SpriteRenderer::SetSprite(const std::shared_ptr<Texture>& sprite)
{
	m_Sprite = sprite;
}

void SpriteRenderer::Serialize(json& out) const
{
	Component::Serialize(out["SpriteRenderer"]);

	const Color& color = GetColor();
	out["SpriteRenderer"] =
	{
		{ "Color",
			{{ "R", color.r },
			 { "G", color.g },
			 { "B", color.b },
			 { "A", color.a }}},
		{ "Texture",  GetTexture()->GetPath() },
		{ "OrderInLayer",  orderInLayer}
	};
}

void SpriteRenderer::Deserialize(json& in)
{
	Component::Deserialize(in);

	Color color;
	color.r = in["Color"]["R"];
	color.g = in["Color"]["G"];
	color.b = in["Color"]["B"];
	color.a = in["Color"]["A"];

	SetColor(color);
	SetSprite(in["Texture"]);
	orderInLayer = in["OrderInLayer"];
}