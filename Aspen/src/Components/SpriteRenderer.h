#pragma once
#include <algorithm>

#include "Component.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Shader.h"
//#include "../src/GameObject/GameObject.h"

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
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	unsigned int m_VAO; // TODO: make a class

	Texture m_Sprite;
	Shader m_Shader;
	Color m_Color;

	friend class Inspector;
public:
	int orderInLayer = 0;

public:
	SpriteRenderer(GameObject* gameObject, Transform* transform);

	void SetColor(const Color& color);
	void SetSprite(const std::string_view path);
	void SetSprite(const Texture& id);

	int GetPixel();
	const Color& GetColor() const { return m_Color; }
	const Texture& GetTexture() const { return m_Sprite; }
	Shader& GetShader() { return m_Shader; }

	void Draw();
};