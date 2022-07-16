#pragma once
#include <memory>
#include <vector>
#include <glad/glad.h>
#include "../Log/Log.h"

class ASPEN Framebuffer
{
private:
	struct ColorAttachment
	{
		GLenum internalFormat;
		GLenum format;
	};

	unsigned int m_ID;
	//unsigned int m_Texture;
	std::vector<ColorAttachment> m_Specifications;
	std::vector<unsigned int> m_ColorAttachments;
	unsigned int m_Width, m_Height;

private:
	void Invalidate();
	void AddColorAttachments();

public:
	Framebuffer();
	Framebuffer(unsigned int width, unsigned int height);

	void Bind();
	void Unbind();

	void DrawBuffers() const;
	void Resize(int width, int height);
	void AddColorAttachment(GLenum internalFormat, GLenum format);
	int ReadPixel(unsigned int attachmentIndex, int x, int y);
	unsigned int GetColorAttachmentID(unsigned int attachment = 0);
	
	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	unsigned int GetID() const { return m_ID; }
	//virtual int Get

	~Framebuffer();
};