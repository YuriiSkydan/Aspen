#pragma once
#include <iostream>

#include "glad/glad.h"
#include "../Core/Core.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Camera.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"

void ASPEN GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);

void ASPEN ErrorCallback(int error, const char* message);

class ASPEN Renderer
{
private:
	inline static VertexBuffer s_VertexBuffer;
	inline static IndexBuffer s_IndexBuffer;
	inline static unsigned int s_VAO;
	inline static std::shared_ptr<Shader> s_StandartShader;

	inline static std::shared_ptr<Shader> s_BoxColliderShader;
	inline static std::shared_ptr<Texture> s_CircleColliderTexture;
public:
	static void Init();
	static void BeginScene(const Matrix3x3f& cameraMatrix);
	static void BeginScene(const Camera* camera);
	static void Draw(const SpriteRenderer* spriteRenderer);
	static void EndScene();

	//Debug
	static void DrawBoxCollider(const Transform* transform, const BoxCollider* boxCollider);
	static void DrawCirlceCollider(const Transform* transform, const CircleCollider* circleCollider);
};