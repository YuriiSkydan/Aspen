#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Core/Core.h"

class ASPEN VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);

	void SetData(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() { return m_ID; }

	~VertexBuffer() = default;
};