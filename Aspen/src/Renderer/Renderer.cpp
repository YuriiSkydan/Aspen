#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Components/Transform.h"
#include "../Components/BoxCollider.h"
#include "../Components/CircleCollider.h"
#include "../Components/Camera.h"

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "[OpenGL Error] (" << type << ") " << message << std::endl;
	//__debugbreak();
}

void ErrorCallback(int error, const char* message)
{
	std::cerr << "GLFW Error [" << error << "] " << message << std::endl;
}

float vertices[] =
{
	 0.5f,  0.5f, 1.0f, 1.0f,  // top right
	 0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f   // top left 
};

unsigned int indices[] =
{
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

void Renderer::Init()
{
	glDebugMessageCallback(DebugMessageCallback, nullptr);
	glfwSetErrorCallback(ErrorCallback);

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	s_VertexBuffer.SetData(vertices, sizeof(vertices));
	s_IndexBuffer.SetData(indices, 6);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(s_VAO);

	s_StandartShader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");
	s_BoxColliderShader = ShaderLibrary::Get()->GetShader("Shaders/BoxColliderShader");
}

void Renderer::BeginScene(const Matrix3x3f& cameraMatrix)
{
	s_StandartShader->Bind();
	s_StandartShader->SetMat3("camera", cameraMatrix);

	s_BoxColliderShader->Bind();
	s_BoxColliderShader->SetMat3("camera", cameraMatrix);
	glBindVertexArray(s_VAO);
}

void Renderer::BeginScene(const Camera* camera)
{
	s_StandartShader->Bind();
	s_StandartShader->SetMat3("camera", camera->GetCameraMatrix());

	s_BoxColliderShader->Bind();
	s_BoxColliderShader->SetMat3("camera", camera->GetCameraMatrix());
	glBindVertexArray(s_VAO);
}

void Renderer::Draw(const SpriteRenderer* spriteRenderer)
{
	s_StandartShader->Bind();

	s_StandartShader->SetBool("flipX", spriteRenderer->flipX);
	s_StandartShader->SetBool("flipY", spriteRenderer->flipY);
	s_StandartShader->SetMat3("transform", spriteRenderer->transform->GetTransformMatrix());

	Color color = spriteRenderer->GetColor();
	s_StandartShader->SetVec4f("spriteColor", color.r, color.g, color.b, color.a);

	spriteRenderer->GetTexture()->Bind(0);
	s_StandartShader->SetInt("sprite", 0);

	s_StandartShader->SetInt("gameObjectID", spriteRenderer->gameObject->GetID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void Renderer::EndScene()
{

}

void Renderer::DrawBoxCollider(const Transform* transform, const BoxCollider* boxCollider)
{
	Vector2f scale(transform->scale);
	scale.x = boxCollider->size.x * 2;
	scale.y = boxCollider->size.y * 2;

	Matrix3x3f transformMatrix = Matrix3x3f(1.0f);

	transformMatrix = MatrixTransform::Translate(transformMatrix, transform->position);
	transformMatrix = MatrixTransform::Scale(transformMatrix, scale);
	transformMatrix = MatrixTransform::Scale(transformMatrix, transform->scale);
	transformMatrix = MatrixTransform::Rotate(transformMatrix, transform->angle);

	Matrix3x3f position = MatrixTransform::Rotate(Matrix3x3f(1.0f), transform->angle);
	position = MatrixTransform::Translate(position, boxCollider->offset);

	transformMatrix[0][2] += position[0][2];
	transformMatrix[1][2] += position[1][2];

	s_BoxColliderShader->Bind();
	s_BoxColliderShader->SetMat3("transform", transformMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawCirlceCollider(const Transform* transform, const CircleCollider* circleCollider)
{
	Vector2f scale(transform->scale);
	scale *= circleCollider->radius * 2;

	Vector2f position(transform->position);
	position += circleCollider->offset;

	Matrix3x3f transformMatrix = Matrix3x3f(1.0f);

	transformMatrix = MatrixTransform::Translate(transformMatrix, position);
	transformMatrix = MatrixTransform::Scale(transformMatrix, scale);

	s_StandartShader->Bind();
	s_StandartShader->SetMat3("transform", transformMatrix);

	Color color(0.0f, 1.0f, 0.0f, 1.0f);
	s_StandartShader->SetVec4f("spriteColor", color.r, color.g, color.b, color.a);

	//TextureLibrary::Get()->GetTexture("Resources/CircleCollider.png")->Bind(0);
	s_StandartShader->SetInt("sprite", 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}