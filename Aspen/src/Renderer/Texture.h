#pragma once
#include "../Core/Core.h"
#include <iostream>
#include <string>
#include <string_view>

class ASPEN Texture
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

	unsigned int GetID() const { return m_ID; }
	const std::string& GetPath() const { return m_Path; }

	~Texture();
};