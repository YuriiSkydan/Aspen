#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>


#include <iostream>

#include "../Core/Core.h"

class ASPEN Texture
{
private:
	unsigned int m_ID;
	std::string m_Path;

	int m_Width, m_Height;
	int m_Channels;

	friend class TextureLibrary;
	friend class std::shared_ptr<Texture>;
public:
	Texture(std::string_view path);

	void Bind(unsigned int slot) const;
	void UnBind() const;

	unsigned int GetID() const { return m_ID; }
	const std::string& GetPath() const { return m_Path; }

	~Texture();
};

class ASPEN TextureLibrary
{
private:
	inline static TextureLibrary* s_Instance = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

public:
	TextureLibrary();
	TextureLibrary(const TextureLibrary& other) = delete;
	const TextureLibrary& operator=(const TextureLibrary& other) = delete;

	static TextureLibrary* Get();
	const std::shared_ptr<Texture>& GetTexture(const std::string& filePath);
};