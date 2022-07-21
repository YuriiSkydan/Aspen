#pragma once
#include <iostream>

#include "glad/glad.h"
#include "../Core/Core.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Camera.h"

void ASPEN GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);

void ASPEN ErrorCallback(int error, const char* message);

class Renderer
{
private:
	inline static VertexBuffer m_VertexBuffer;
	inline static IndexBuffer m_IndexBuffer;
	inline static unsigned int m_VAO;
	inline static std::shared_ptr<Shader> m_StandartShader;

public:
	static void Init();
	static void BeginScene(const Matrix3x3f& cameraMatrix);
	static void BeginScene(const Camera* camera);
	static void Draw(const SpriteRenderer* spriteRenderer);
	static void EndScene();
};