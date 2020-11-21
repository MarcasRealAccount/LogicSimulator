#include "Rendering/Apis/OpenGL/GLHelpers.h"
#include <iostream>

void GLDebugMessenger(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "GL Error: " << source << ", " << type << ", " << id << ", " << severity << std::endl << message << std::endl;
}

void glSetupDebugMessenger() {
	glDebugMessageCallback(GLDebugMessenger, nullptr);
}