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
class PolygonCollider;

void ASPEN GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);

void ASPEN ErrorCallback(int error, const char* message);

class ASPEN Renderer
{
private:
	inline static std::shared_ptr<Shader> s_StandartShader;
	inline static std::shared_ptr<Shader> s_BoxColliderShader;
	inline static std::shared_ptr<Shader> s_CircleColliderShader;
	inline static std::shared_ptr<Shader> s_LineShader;

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
	static void DrawLine(const Vector2f from, const Vector2f to);
	static void DrawBoxCollider(const BoxCollider* boxCollider, const Matrix3x3f& cameraMatrix);
	static void DrawCirlceCollider(const CircleCollider* circleCollider, const Matrix3x3f& cameraMatrix);
	static void DrawPolygonCollider(const PolygonCollider* polygonCollider, const Matrix3x3f& cameraMatrix);
};