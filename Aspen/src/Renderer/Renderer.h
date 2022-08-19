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
	//inline static VertexBuffer s_VertexBuffer;
	//inline static IndexBuffer s_IndexBuffer;
	//inline static unsigned int s_VAO;
	inline static std::shared_ptr<Shader> s_StandartShader;
	inline static std::shared_ptr<Shader> s_BoxColliderShader;


	inline static unsigned int s_MaxQuads = 1;
private:
	static void DrawQuad(Transform* transform);

public:
	static void Init();
	static void BeginScene(const Matrix3x3f& cameraMatrix);
	static void Draw(const SpriteRenderer* spriteRenderer);
	static void EndScene();

	//Debug
	static void DrawBoxCollider(const Transform* transform, const BoxCollider* boxCollider);
	static void DrawCirlceCollider(const Transform* transform, const CircleCollider* circleCollider);
};