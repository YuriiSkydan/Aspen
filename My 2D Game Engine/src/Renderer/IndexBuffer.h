#pragma once

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

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

	~IndexBuffer() = default;
};

#endif