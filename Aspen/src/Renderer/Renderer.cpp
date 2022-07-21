#include "Renderer.h"
#include "../Components/Transform.h"

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "[OpenGL Error] (" << type << ") " << message << std::endl;
	__debugbreak();
}

void ErrorCallback(int error, const char* message)
{
	std::cerr << "GLFW Error [" << error << "] " << message << std::endl;
}

float square[]
{
	 0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f,  // top left
	 0.5f,  0.5f, 1.0f, 1.0f   // top right 
};

unsigned int indicies[]
{
	0, 1, 2,
	0, 3, 2
};

void Renderer::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	m_VertexBuffer.SetData(square, sizeof(square));
	m_IndexBuffer.SetData(indicies, 6);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(m_VAO);

	m_StandartShader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");
}

void Renderer::BeginScene(const Matrix3x3f& cameraMatrix)
{
	m_StandartShader->SetMat3("camera", cameraMatrix);
	glBindVertexArray(m_VAO);
}

void Renderer::BeginScene(const Camera* camera)
{
	m_StandartShader->SetMat3("camera", camera->GetCameraMatrix());
	glBindVertexArray(m_VAO);
}

void Renderer::Draw(const SpriteRenderer* spriteRenderer)
{
	m_StandartShader->Bind();
	m_StandartShader->SetBool("flipX", spriteRenderer->flipX);
	m_StandartShader->SetBool("flipY", spriteRenderer->flipY);
	m_StandartShader->SetMat3("transform", spriteRenderer->transform->GetTransformMatrix());
	
	Color color = spriteRenderer->GetColor();
	m_StandartShader->SetVec4f("spriteColor", color.r, color.g, color.b, color.a);

	spriteRenderer->GetTexture().Bind(0);
	m_StandartShader->SetInt("sprite", 0);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void Renderer::EndScene()
{

}