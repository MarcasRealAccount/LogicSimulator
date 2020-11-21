#pragma once

#include <stdint.h>
#include <string>
#include <map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Renderer;

struct WindowData {
public:
	uint32_t x = 0, y = 0;
	uint32_t w = 0, h = 0;
	uint32_t fw = 0, fh = 0;

	bool maximized = false, iconified = false;
	bool fc = false;
};

class Window {
public:
	void DefaultWindowHints();
	void WindowHint(uint32_t hint, uint32_t value);

	bool Create();
	void Destroy();
	bool IsWindowCreated() const;

	void RequestWindowClose();
	bool ShouldWindowClose() const;
	bool HasFramebufferChanged() const;

	const WindowData& GetData() const;
	void SetPos(uint32_t x, uint32_t y);
	void SetSize(uint32_t w, uint32_t h);
	void GetPos(uint32_t* x, uint32_t* y) const;
	void GetSize(uint32_t* w, uint32_t* h) const;
	void GetFramebufferSize(uint32_t* fw, uint32_t* fh) const;

	friend Renderer;

public:
	static void PollEvents();

public:	// OpenGL functions:
	void MakeContextCurrent();
	void SwapBuffers();

public:	// OpenGL functions:
	static void SwapInterval(uint32_t interval);

private:
	static void ErrorCallback(int errorCode, const char* description);

	static void WindowPosCallback(GLFWwindow* windowPtr, int x, int y);
	static void WindowSizeCallback(GLFWwindow* windowPtr, int width, int height);
	static void FramebufferSizeCallback(GLFWwindow* windowPtr, int width, int height);

private:
	GLFWwindow* windowPtr = nullptr;

	WindowData data;
	std::string title;

	std::map<uint32_t, uint32_t> windowHints;
};