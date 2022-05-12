#pragma once

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);

	void SetData(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);

	void Bind() const;
	void Unbind() const;

	~VertexBuffer() = default;
};

#endif