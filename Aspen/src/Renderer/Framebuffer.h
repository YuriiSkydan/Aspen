#pragma once
#include <memory>
#include <vector>
#include <glad/glad.h>
#include "../Log/Log.h"

class ASPEN Framebuffer
{
private:
	unsigned int m_ID;
	//unsigned int m_Texture;
	std::vector<unsigned int> m_ColorAttachments;
	unsigned int m_Width, m_Heigth;

public:
	Framebuffer();

	void Bind();
	void Unbind();

	void DrawBuffers() const;
	void Resize(int width, int heigth);
	int ReadPixel(unsigned int attachmentIndex, int x, int y);
	void AddColorAttachment(GLenum internalFormat, GLenum format);
	unsigned int GetColorAttachmentID(unsigned int attachment = 0);
	//int GetPixel(int x, int y);
	unsigned int GetID() { return m_ID; }
	//virtual int Get

	~Framebuffer();
};