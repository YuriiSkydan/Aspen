#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
	unsigned int m_ID;
	unsigned int m_Count;
public:
	IndexBuffer() = default;
	IndexBuffer(const void* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);

	void SetData(const void* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);

	void Bind() const;
	void Unbind() const;

	unsigned int GetID() { return m_ID; }

	~IndexBuffer() = default;
};