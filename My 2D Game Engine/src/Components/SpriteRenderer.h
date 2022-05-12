#pragma once

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERRE_H

#include "glad/glad.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/Texture.h"
#include "../Components/Component.h"
#include "../Renderer/Shader.h"
//#include "../src/GameObject/GameObject.h"

#include <algorithm>

struct Color
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	Color() = default;
	Color(float value) : r(value), g(value), b(value), a(value) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

class SpriteRenderer : public Component
{
private:
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	unsigned int m_VAO;

	//Texture m_Sprite;
	Shader m_Shader;
	Color m_Color;

public:
	int orderInLayer = 0;

private:
	void UpdateGui() override;

public:
	SpriteRenderer(GameObject* gameObject, Transform* transform);

	void SetColor(const Color& color);
	void SetSprite(std::string_view& path);
	void SetSprite(Texture id);

	Shader& GetShader() { return m_Shader; }

	void Draw();
};

#endif