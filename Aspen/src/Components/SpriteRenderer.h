#pragma once
#include "Component.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Shader.h"

struct ASPEN Color
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	Color() = default;
	Color(float value) : r(value), g(value), b(value), a(value) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

class ASPEN SpriteRenderer : public Component
{
private:
	std::shared_ptr<Texture> m_Sprite;
	std::shared_ptr<Shader> m_Shader;
	Color m_Color;

public:
	bool flipX = false;
	bool flipY = false;
	int orderInLayer = 0;

public:
	SpriteRenderer(GameObject* gameObject, Transform* transform);

	void Reset() override;

	void SetColor(const Color& color);
	void SetSprite(const std::string_view path);
	void SetSprite(const std::shared_ptr<Texture>& sprite);

	int GetPixel();

	const Color& GetColor() const { return m_Color; }
	const std::shared_ptr<Texture>& GetTexture() const { return m_Sprite; }
	const std::shared_ptr<Shader>& GetShader() { return m_Shader; }

	void Serialize(json& out) const override;
	void Deserialize(json& in) override;

	~SpriteRenderer() = default;
};