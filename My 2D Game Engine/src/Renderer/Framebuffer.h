#pragma once

#include <memory>
#include <vector>

class Framebuffer
{
private:
	unsigned int m_ID;
	unsigned int m_Texture;

	unsigned int m_Width, m_Heigth;

public:
	Framebuffer();

	void Bind();
	void Unbind();

	void Resize(int width, int heigth);
	int ReadPixel(int attachmentIndex, int x, int y);

	unsigned int GetTexture() { return m_Texture; }
	static std::shared_ptr<Framebuffer> Create();
	//virtual int Get
};

