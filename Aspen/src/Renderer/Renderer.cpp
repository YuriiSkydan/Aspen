#include <unordered_map>
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

//float vertices[] =
//{
//	 0.5f,  0.5f, 1.0f, 1.0f,  // top right
//	 0.5f, -0.5f, 1.0f, 0.0f,  // bottom right
//	-0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
//	-0.5f,  0.5f, 0.0f, 1.0f   // top left 
//};
//
//unsigned int indices[] =
//{
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

#define ExperimentalBatchRendering

#ifdef BatchRendering
struct QuadVertex
{
	Vector2f position;
	Color color;
	Vector2f texCoords;
	float texID;
	float gameObjectID;
};

struct RenderData
{
	static const unsigned int MaxQuads = 10000;
	static const unsigned int MaxVertices = MaxQuads * 4;
	static const unsigned int MaxIndices = MaxQuads * 6;
	static const unsigned int MaxTextureSlots = 32;
	static const unsigned int QuadVertices = 4;

	Vector2f quadVertexPositions[QuadVertices];
	//Move to SpriteRenderer Later
	Vector2f texturePositions[QuadVertices];
	unsigned int indices[MaxIndices];
	std::vector<QuadVertex> vertices;

	unsigned int vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	unsigned int quads = 0;
	unsigned int vertexCount = 0;
	unsigned int drawCalls = 0;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	unsigned int offset = 0;
	for (size_t i = 0; i < RenderData::MaxIndices; i += 6)
	{
		s_RenderData.indices[i + 0] = 0 + offset;
		s_RenderData.indices[i + 1] = 1 + offset;
		s_RenderData.indices[i + 2] = 2 + offset;

		s_RenderData.indices[i + 3] = 2 + offset;
		s_RenderData.indices[i + 4] = 3 + offset;
		s_RenderData.indices[i + 5] = 0 + offset;

		offset += 4;
	}

	s_RenderData.quadVertexPositions[0] = { 0.5f,  0.5f };
	s_RenderData.quadVertexPositions[1] = { 0.5f, -0.5f };
	s_RenderData.quadVertexPositions[2] = { -0.5f, -0.5f };
	s_RenderData.quadVertexPositions[3] = { -0.5f,  0.5f };

	s_RenderData.texturePositions[0] = { 1.0f, 1.0f };
	s_RenderData.texturePositions[1] = { 1.0f, 0.0f };
	s_RenderData.texturePositions[2] = { 0.0f, 0.0f };
	s_RenderData.texturePositions[3] = { 0.0f, 1.0f };

	glGenVertexArrays(1, &s_RenderData.vertexArray);
	glBindVertexArray(s_RenderData.vertexArray);

	s_RenderData.quads++;

	s_RenderData.vertexBuffer.SetData(nullptr, RenderData::MaxVertices * sizeof(QuadVertex), GL_DYNAMIC_DRAW);
	s_RenderData.indexBuffer.SetData(s_RenderData.indices, RenderData::MaxIndices);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texCoords));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texID));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, gameObjectID));
	glEnableVertexAttribArray(4);

	s_StandartShader = ShaderLibrary::Get()->GetShader("Shaders/StandartShader");

	s_RenderData.vertices.resize(s_RenderData.MaxVertices);
}

void Renderer::BeginScene(const Matrix3x3f& cameraMatrix)
{
	s_RenderData.quads = 0;
	s_RenderData.vertexCount = 0;
	s_RenderData.drawCalls = 0;

	s_StandartShader->Bind();
	s_StandartShader->SetMat3("camera", cameraMatrix);
}

void Renderer::Draw(const SpriteRenderer* spriteRenderer)
{
	Matrix3x3f transform = spriteRenderer->transform->GetTransformMatrix();
	Color color = spriteRenderer->GetColor();
	unsigned int ID = spriteRenderer->gameObject->GetID();

	QuadVertex vertex;
	for (size_t i = 0; i < RenderData::QuadVertices; i++)
	{
		vertex.position = transform * s_RenderData.quadVertexPositions[i];
		vertex.position.x += transform[0][2];
		vertex.position.y += transform[1][2];
		vertex.color = color;
		vertex.texCoords = s_RenderData.texturePositions[i];
		vertex.gameObjectID = ID;
		vertex.texID = 0;

		size_t index = s_RenderData.vertexCount;
		s_RenderData.vertices[index] = vertex;
		s_RenderData.vertexCount++;
	}

	s_RenderData.quads++;
}

void Renderer::StartBatch()
{
}

void Renderer::Flush()
{
}

void Renderer::EndScene()
{
	s_RenderData.vertexBuffer.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(QuadVertex) * s_RenderData.vertexCount, s_RenderData.vertices.data());
	glDrawElements(GL_TRIANGLES, s_RenderData.quads * 6, GL_UNSIGNED_INT, nullptr);
}
void Renderer::ShutDown()
{
}
#endif

//Expertimental Batch Rendering
#ifdef ExperimentalBatchRendering
struct QuadVertex
{
	Vector2f vertexPosition;
	Vector2f quadPosition;
	Vector2f scale;
	float angle;
	Color color;
	Vector2f texCoords;
	float texIndex;
	float gameObjectID;
};

struct RenderData
{
	static const unsigned int MaxQuads = 10000;
	static const unsigned int MaxVertices = MaxQuads * 4;
	static const unsigned int MaxIndices = MaxQuads * 6;
	static const unsigned int MaxTextureSlots = 32;
	static const unsigned int QuadVertices = 4;

	Vector2f quadVertexPositions[QuadVertices];
	//Move to SpriteRenderer Later
	Vector2f texturePositions[QuadVertices];
	unsigned int indices[MaxIndices];
	std::vector<QuadVertex> vertices;
	std::unordered_map<Texture*, unsigned int> textures;

	unsigned int vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	unsigned int quads = 0;
	unsigned int vertexCount = 0;
	unsigned int textureCount = 0;
	unsigned int drawCalls = 0;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	unsigned int offset = 0;
	for (size_t i = 0; i < RenderData::MaxIndices; i += 6)
	{
		s_RenderData.indices[i + 0] = 0 + offset;
		s_RenderData.indices[i + 1] = 1 + offset;
		s_RenderData.indices[i + 2] = 2 + offset;

		s_RenderData.indices[i + 3] = 2 + offset;
		s_RenderData.indices[i + 4] = 3 + offset;
		s_RenderData.indices[i + 5] = 0 + offset;

		offset += 4;
	}

	s_RenderData.quadVertexPositions[0] = { 0.5f,  0.5f };
	s_RenderData.quadVertexPositions[1] = { 0.5f, -0.5f };
	s_RenderData.quadVertexPositions[2] = { -0.5f, -0.5f };
	s_RenderData.quadVertexPositions[3] = { -0.5f,  0.5f };

	s_RenderData.texturePositions[0] = { 1.0f, 1.0f };
	s_RenderData.texturePositions[1] = { 1.0f, 0.0f };
	s_RenderData.texturePositions[2] = { 0.0f, 0.0f };
	s_RenderData.texturePositions[3] = { 0.0f, 1.0f };

	glGenVertexArrays(1, &s_RenderData.vertexArray);
	glBindVertexArray(s_RenderData.vertexArray);

	s_RenderData.quads++;

	s_RenderData.vertexBuffer.SetData(nullptr, RenderData::MaxVertices * sizeof(QuadVertex), GL_DYNAMIC_DRAW);
	s_RenderData.indexBuffer.SetData(s_RenderData.indices, RenderData::MaxIndices);

	//Vertex Position
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, vertexPosition));
	glEnableVertexAttribArray(0);

	//Quad position
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, quadPosition));
	glEnableVertexAttribArray(1);

	//Scale
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, scale));
	glEnableVertexAttribArray(2);

	//Angle
	glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, angle));
	glEnableVertexAttribArray(3);

	//Color
	glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, color));
	glEnableVertexAttribArray(4);

	//Texture Cords
	glVertexAttribPointer(5, 2, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texCoords));
	glEnableVertexAttribArray(5);

	//Texture ID
	glVertexAttribPointer(6, 1, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texIndex));
	glEnableVertexAttribArray(6);

	//GameObject ID
	glVertexAttribPointer(7, 1, GL_FLOAT, false, sizeof(QuadVertex), (void*)offsetof(QuadVertex, gameObjectID));
	glEnableVertexAttribArray(7);

	s_StandartShader = ShaderLibrary::Get()->GetShader("Shaders/FastStandartShader");
	s_BoxColliderShader = ShaderLibrary::Get()->GetShader("Shaders/BoxColliderShader");

	s_RenderData.vertices.resize(s_RenderData.MaxVertices);
	s_RenderData.textures.reserve(s_RenderData.MaxTextureSlots);

}

void Renderer::BeginScene(const Matrix3x3f& cameraMatrix)
{
	s_StandartShader->Bind();
	s_StandartShader->SetMat3("camera", cameraMatrix);

	int indexes[RenderData::MaxTextureSlots];
	for (size_t i = 0; i < RenderData::MaxTextureSlots; i++)
		indexes[i] = i;

	s_StandartShader->SetArray1iv("u_Textures", RenderData::MaxTextureSlots, indexes);

	StartBatch();
}

void Renderer::Draw(const SpriteRenderer* spriteRenderer)
{
	Transform* transform = spriteRenderer->transform;
	Color color = spriteRenderer->GetColor();
	unsigned int ID = spriteRenderer->gameObject->GetID();
	unsigned int texIndex = 0;

	auto& textures = s_RenderData.textures;
	if (textures.find(spriteRenderer->GetTexture().get()) == textures.end())
	{
		unsigned int textureCount = s_RenderData.textureCount;
		textures.insert({ spriteRenderer->GetTexture().get(), textureCount });
		
		s_RenderData.textureCount++;
	}

	texIndex = textures[spriteRenderer->GetTexture().get()];

	QuadVertex vertex;
	for (size_t i = 0; i < RenderData::QuadVertices; i++)
	{
		vertex.vertexPosition = s_RenderData.quadVertexPositions[i];
		vertex.quadPosition = transform->position;
		vertex.scale = transform->scale;
		vertex.angle = transform->angle;
		vertex.color = color;
		vertex.texCoords = s_RenderData.texturePositions[i];
		vertex.texIndex = texIndex;
		vertex.gameObjectID = ID;

		size_t index = s_RenderData.vertexCount;
		s_RenderData.vertices[index] = vertex;
		s_RenderData.vertexCount++;
	}

	s_RenderData.quads++;

	if (s_RenderData.quads == RenderData::MaxQuads ||
		s_RenderData.textureCount == RenderData::MaxTextureSlots)
	{
		Batch();
	}
}

void Renderer::StartBatch()
{
	s_RenderData.textures.clear();
	s_RenderData.quads = 0;
	s_RenderData.vertexCount = 0;
	s_RenderData.textureCount = 0;
	s_RenderData.drawCalls = 0;
}

void Renderer::Batch()
{
	Flush();
	StartBatch();
}

void Renderer::Flush()
{
	auto& textures = s_RenderData.textures;
	for (auto it = textures.begin(); it != textures.end(); ++it)
		it->first->Bind(it->second);

	s_RenderData.vertexBuffer.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(QuadVertex) * s_RenderData.vertexCount, s_RenderData.vertices.data());
	glDrawElements(GL_TRIANGLES, s_RenderData.quads * 6, GL_UNSIGNED_INT, nullptr);

	for (auto it = textures.begin(); it != textures.end(); ++it)
		it->first->UnBind();
}

void Renderer::EndScene()
{
	Flush();
}

void Renderer::ShutDown()
{
	s_StandartShader.reset();
	s_BoxColliderShader.reset();
}

void Renderer::DrawBoxCollider(const Transform* transform, const BoxCollider* boxCollider, const Matrix3x3f& cameraMatrix)
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
	s_BoxColliderShader->SetMat3("camera", cameraMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
#endif