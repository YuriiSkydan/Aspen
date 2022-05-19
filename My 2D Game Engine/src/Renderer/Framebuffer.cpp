#include "Framebuffer.h"

#include <glad/glad.h>

Framebuffer::Framebuffer()
{
	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	//glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::Resize(int width, int heigth)
{
	//if (width == 0 || heigth == 0 || width )
}

int Framebuffer::ReadPixel(int attachmentIndex, int x, int y)
{
	//glBindTexture(GL_TEXTURE_2D, m_Texture);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	unsigned int pixelData;
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_INT, &pixelData);

	return pixelData;
}

//int Framebuffer::GetPixel(int x, int y)
//{
//	int pixelData;
//	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData;
//	return pixelData;
//}
