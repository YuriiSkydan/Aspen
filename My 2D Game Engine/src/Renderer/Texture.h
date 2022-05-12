#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"
#include <iostream>
#include <string>
#include <string_view>

class Texture
{
private:
	unsigned int m_ID;
	std::string m_Path;

	int m_Width, m_Height;
	int m_Channels;

public:
	Texture(std::string_view path);

	void Bind(unsigned int slot) const;
	void UnBind() const;

	~Texture() = default;
};

#endif