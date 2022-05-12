#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count, GLenum usage)
{
	SetData(data, count, usage);
}

void IndexBuffer::SetData(const void* data, unsigned int count, GLenum usage)
{
	m_Count = count;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, usage);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}