#pragma once
#include <../Libraries/include/glad/glad.h>
#include <../Libraries/include/GLFW/glfw3.h>

#include <memory>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "../Math/Matrix.h"
#include "../Core/Core.h"
//#include "../vendor/glm/glm.hpp"
//#include "../vendor/glm/gtc/type_ptr.hpp"

class ASPEN Shader
{
private:
	unsigned int m_ID;

	friend class ShaderLibrary;
	friend class std::shared_ptr<Shader>;
private:
	unsigned int CompileShader(const char* source, int shaderType);
	unsigned int CreateProgram(unsigned int vShader, unsigned int fShader);
	int GetUniformLocation(std::string_view name);

public:
	Shader(std::string_view vShaderPath, std::string_view fShaderPath);

	void Bind() const;
	void UnBind() const;

	void SetInt(std::string_view name, int value);
	void SetFloat(std::string_view name, float value);
	void SetBool(std::string_view name, int value);

	void SetMat3(std::string_view name, const Matrix3x3f& matrix, bool transpose = false);

	void SetVec3i(std::string_view name, int v0, int v1, int v2);
	void SetVec3f(std::string_view name, float v0, float v1, float v2);
	void SetVec4i(std::string_view name, int v0, int v1, int v2, int v3);
	void SetVec4f(std::string_view name, float v0, float v1, float v2, float v3);

	void SetArray1iv(std::string_view name, unsigned int count, int* arr);

	unsigned int GetID() { return m_ID; }

	~Shader();
};

class ASPEN ShaderLibrary
{
private:
	inline static ShaderLibrary* s_Instance = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;

public:
	static ShaderLibrary* Get();
	const std::shared_ptr<Shader>& GetShader(const std::string& filePath);
};