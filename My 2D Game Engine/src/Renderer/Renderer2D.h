#pragma once
#include "../Math/Matrix.h"
#include "../Components/SpriteRenderer.h"

class Renderer2D
{
public:
	static void Init();
	static void Shutdown();

	//static void BeginScene(const Camera& camera, const Matrix3x3f& transform);
	//static void BeginScene(const EditorCamera& camera);
	static void EndScene();
	static void Flush();

	static void DrawQuad(const Vector2f& position, const Vector2f& size, const Color& color);
	static void DrawQuad(const Vector2f& position, const Vector2f& size, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.0f, const Color& tintColor = Color(1.0f));
};

