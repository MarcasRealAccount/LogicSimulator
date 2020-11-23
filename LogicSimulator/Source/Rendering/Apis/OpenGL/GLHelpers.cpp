#include "Rendering/Apis/OpenGL/GLHelpers.h"
#include <iostream>

Logger OpenGLLogger("OpenGL");

void GLDebugMessenger(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		OpenGLLogger.LogInfo("%d, %d, %d, %s", source, type, id, message);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		OpenGLLogger.LogWarning("%d, %d, %d, %s", source, type, id, message);
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		OpenGLLogger.LogError("%d, %d, %d, %s", source, type, id, message);
		break;
	}
}

void glSetupDebugMessenger() {
	glDebugMessageCallback(GLDebugMessenger, nullptr);
}