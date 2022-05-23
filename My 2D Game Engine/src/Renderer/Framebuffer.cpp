#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::DrawBuffers() const
{
	std::vector<GLenum> attachments;
	for (size_t i = 0; i < m_ColorAttachments.size(); i++)
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);

	glDrawBuffers(m_ColorAttachments.size(), attachments.data());
}

void Framebuffer::Resize(int width, int heigth)
{
	//if (width == 0 || heigth == 0 || width )
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 1920, 1080, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size(), GL_TEXTURE_2D, texture, 0);
	m_ColorAttachments.push_back(texture);
}

unsigned int Framebuffer::GetColorAttachmentID(unsigned int attachment)
{
	return m_ColorAttachments[attachment];
}

//int Framebuffer::GetPixel(int x, int y)
//{
//	int pixelData;
//	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData;
//	return pixelData;
//}
