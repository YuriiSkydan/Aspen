#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "../Math/Matrix.h"

//#include "../vendor/glm/glm.hpp"
//#include "../vendor/glm/gtc/type_ptr.hpp"

class Shader
{
private:
	unsigned int m_ID;

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
	//void SetVec3f(std::string_view name, const glm::vec3& vec);

	void SetVec4i(std::string_view name, int v0, int v1, int v2, int v3);
	void SetVec4f(std::string_view name, float v0, float v1, float v2, float v3);

	//void SetMat4(std::string_view name, const glm::mat4& mat);
	unsigned int GetID() { return m_ID; }

	~Shader() = default;
};

#endif