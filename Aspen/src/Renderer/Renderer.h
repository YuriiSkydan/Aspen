#pragma once
#include "glad/glad.h"
#include "../Components/SpriteRenderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"

class Transform;
class Camera;
class CircleCollider;
class BoxCollider;

void ASPEN GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);

void ASPEN ErrorCallback(int error, const char* message);

class ASPEN Renderer
{
private:
	inline static std::shared_ptr<Shader> s_StandartShader;
	inline static std::shared_ptr<Shader> s_BoxColliderShader;
	inline static std::shared_ptr<Shader> s_CircleColliderShader;

public:
	static void Init();
	static void BeginScene(const Matrix3x3f& cameraMatrix);
	static void Draw(const SpriteRenderer* spriteRenderer);
	static void Batch();
	static void StartBatch();
	static void Flush();
	static void EndScene();
	static void ShutDown();

	//Debug
	static void DrawBoxCollider(const Transform* transform, const BoxCollider* boxCollider, const Matrix3x3f& cameraMatrix);
	static void DrawCirlceCollider(const Transform* transform, const CircleCollider* circleCollider, const Matrix3x3f& cameraMatrix);
};