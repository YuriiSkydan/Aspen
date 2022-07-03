#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage)
{
	SetData(data, size, usage);
}

void VertexBuffer::SetData(const void* data, unsigned int size, GLenum usage)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}