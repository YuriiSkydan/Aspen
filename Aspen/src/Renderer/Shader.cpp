#include "Shader.h"

unsigned int Shader::CompileShader(const char* source, int shaderType)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(shader, length, 0, message);

		std::cerr << "ERROR::SHADER::" << ((shaderType == GL_VERTEX_SHADER) ? ("VERTEX") : ("FRAGMENT"));
		std::cerr << "::COMPILATION_FAILED!!!\n" << message << std::endl;

		delete[] message;
	}

	return shader;
}

unsigned int Shader::CreateProgram(unsigned int vShader, unsigned int fShader)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetProgramInfoLog(program, length, 0, message);

		std::cerr << "ERROR::PROGRAM::LIKING_FAILED!!!\n";

		delete[] message;
	}

	return program;
}

int Shader::GetUniformLocation(std::string_view name)
{
	int location = glGetUniformLocation(m_ID, name.data());

	if (location == -1)
		std::cerr << "[Error] Unifomr is not found (" << name << ")\n";

	return location;
}

Shader::Shader(std::string_view vShaderPath, std::string_view fShaderPath)
{
	std::string vShaderCode;
	std::string fShaderCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		vShaderFile.open(vShaderPath.data());
		fShaderFile.open(fShaderPath.data());

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderCode = vShaderStream.str();
		fShaderCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	unsigned int vShader = CompileShader(vShaderCode.c_str(), GL_VERTEX_SHADER);
	unsigned int fShader = CompileShader(fShaderCode.c_str(), GL_FRAGMENT_SHADER);
	m_ID = CreateProgram(vShader, fShader);
}

void Shader::Bind() const
{
	glUseProgram(m_ID);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::SetInt(std::string_view name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(std::string_view name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetBool(std::string_view name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetMat3(std::string_view name, const Matrix3x3f& matrix, bool transpose)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, (float*)(&matrix));
}

void Shader::SetVec3i(std::string_view name, int v0, int v1, int v2)
{
	glUniform3i(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetVec3f(std::string_view name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetVec4i(std::string_view name, int v0, int v1, int v2, int v3)
{
	glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetVec4f(std::string_view name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}


const std::shared_ptr<Shader>& ShaderLibrary::GetShader(const std::string& filePath)
{
	if (m_Shaders.find(filePath) != m_Shaders.end())
		return m_Shaders[filePath];
	
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(filePath + ".vs", filePath + ".fs");
	m_Shaders.insert({ filePath, shader });
	return shader;
}

ShaderLibrary* ShaderLibrary::Get()
{
	static ShaderLibrary library;
	s_Instance = &library;

	return s_Instance;
}
