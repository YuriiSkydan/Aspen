#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "glad/glad.h"
#include <iostream>

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length, const GLchar* message, const void* userParam);

void ErrorCallback(int error, const char* message);

#endif