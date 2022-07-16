#include "Framebuffer.h"
#include <iostream>
void Framebuffer::Invalidate()
{
	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());

		m_ColorAttachments.clear();
	}

	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	auto specifications = m_Specifications;
	m_Specifications.clear();

	for (size_t i = 0; i < specifications.size(); i++)
	{
		AddColorAttachment(specifications[i].internalFormat, specifications[i].format);
	}
	
	DrawBuffers();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::Framebuffer()
	:m_Width(0), m_Height(0)
{
	Invalidate();
}

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
	:m_Width(width), m_Height(height)
{
	Invalidate();
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::DrawBuffers() const
{
	std::vector<GLenum> attachments;
	for (size_t i = 0; i < m_ColorAttachments.size(); i++)
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

	glDrawBuffers(m_ColorAttachments.size(), attachments.data());
}

void Framebuffer::Resize(int width, int height)
{
	if (width == 0 || height == 0)
		return;

	m_Width = width;
	m_Height = height;

	Invalidate();
}

int Framebuffer::ReadPixel(unsigned int attachmentIndex, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

	return pixelData;
}

void Framebuffer::AddColorAttachment(GLenum internalFormat, GLenum format)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size(), GL_TEXTURE_2D, texture, 0);
	m_ColorAttachments.push_back(texture);

	m_Specifications.push_back({internalFormat, format});
}

unsigned int Framebuffer::GetColorAttachmentID(unsigned int attachment)
{
	return m_ColorAttachments[attachment];
}

Framebuffer::~Framebuffer()
{
	INFO("Framebuffer Destructor");
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
}