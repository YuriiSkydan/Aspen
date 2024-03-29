#include "Texture.h"
#include <glad/glad.h>
#include "stb_image/stb_image.h"

#include <iostream>

Texture::Texture(std::string_view path)
	:m_Path(path)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.data(), &m_Width, &m_Height, &m_Channels, 0);

	if (data != nullptr)
	{
		GLenum internalFormat = 0, dataFormat = 0;
		if (m_Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Failed to load texture!!!\n";
	}
}

void Texture::Bind(unsigned int slot) const
{
	glBindTextureUnit(slot, m_ID);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	//Think it's not a good practive but I haven't came up with something better
	if (TextureLibrary::Get() != nullptr)
		TextureLibrary::Get()->Erase(m_Path);

	glDeleteTextures(1, &m_ID);
}

TextureLibrary* TextureLibrary::Get()
{
	static TextureLibrary textureLibrary;
	s_Instance = &textureLibrary;
	return s_Instance;
}

void TextureLibrary::Erase(const std::string& filePath)
{
	if (m_Textures.find(filePath) != m_Textures.end())
		m_Textures.erase(filePath);
}

void TextureLibrary::GetTexture(const std::string& filePath, std::shared_ptr<Texture>& texturePtr)
{
	if (m_Textures.find(filePath) != m_Textures.end())
	{
		texturePtr = m_Textures[filePath].lock();
		return;
	}

	texturePtr = std::make_shared<Texture>(filePath);
	m_Textures.insert({ filePath, texturePtr });
}