#include "Renderer.h"

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "[OpenGL Error] (" << type << ") " << message << std::endl;
    __debugbreak();
}

void ErrorCallback(int error, const char* message)
{
    std::cerr << "GLFW Error [" << error << "] " << message << std::endl;
}