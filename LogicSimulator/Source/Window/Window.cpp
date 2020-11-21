#include "Window/Window.h"

Logger Window::logger("Window");

void Window::DefaultWindowHints() {
	this->windowHints.clear();
}

void Window::WindowHint(uint32_t hint, uint32_t value) {
	this->windowHints.insert_or_assign(hint, value);
}

bool Window::Create() {
	glfwSetErrorCallback(Window::ErrorCallback);

	if (!glfwInit()) return false;

	glfwDefaultWindowHints();
	for (auto& hint : this->windowHints) glfwWindowHint(static_cast<int>(hint.first), static_cast<int>(hint.second));

	this->windowPtr = glfwCreateWindow(this->data.w, this->data.h, this->title.c_str(), nullptr, nullptr);
	if (!this->windowPtr) return false;

	int32_t fw, fh;
	glfwGetFramebufferSize(this->windowPtr, &fw, &fh);
	this->data.fw = static_cast<uint32_t>(fw);
	this->data.fh = static_cast<uint32_t>(fh);

	glfwSetWindowUserPointer(this->windowPtr, this);
	glfwSetWindowPosCallback(this->windowPtr, Window::WindowPosCallback);
	glfwSetWindowSizeCallback(this->windowPtr, Window::WindowSizeCallback);
	glfwSetFramebufferSizeCallback(this->windowPtr, Window::FramebufferSizeCallback);

	return true;
}

void Window::Destroy() {
	glfwDestroyWindow(this->windowPtr);
	glfwTerminate();
}

bool Window::IsWindowCreated() const {
	return this->windowPtr;
}

void Window::RequestWindowClose() {
	glfwSetWindowShouldClose(this->windowPtr, GLFW_TRUE);
}

bool Window::ShouldWindowClose() const {
	return glfwWindowShouldClose(this->windowPtr);
}

bool Window::HasFramebufferChanged() const {
	return this->data.fc;
}

const WindowData& Window::GetData() const {
	return this->data;
}

void Window::SetPos(uint32_t x, uint32_t y) {
	if (IsWindowCreated()) {
		glfwSetWindowPos(this->windowPtr, x, y);
	} else {
		this->data.x = x;
		this->data.y = y;
	}
}

void Window::SetSize(uint32_t w, uint32_t h) {
	if (IsWindowCreated()) {
		glfwSetWindowSize(this->windowPtr, w, h);
	} else {
		this->data.w = w;
		this->data.h = h;
	}
}

void Window::GetPos(uint32_t* x, uint32_t* y) const {
	*x = this->data.x;
	*y = this->data.y;
}

void Window::GetSize(uint32_t* w, uint32_t* h) const {
	*w = this->data.w;
	*h = this->data.h;
}

void Window::GetFramebufferSize(uint32_t* fw, uint32_t* fh) const {
	*fw = this->data.fw;
	*fh = this->data.fh;
}

void Window::MakeContextCurrent() {
	glfwMakeContextCurrent(this->windowPtr);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(this->windowPtr);
	this->data.fc = false;
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::SwapInterval(uint32_t interval) {
	glfwSwapInterval(interval);
}

void Window::ErrorCallback(int errorCode, const char* description) {
	Window::logger.LogError("GLFW Error: %d\n\t%s", errorCode, description);
}

void Window::WindowPosCallback(GLFWwindow* windowPtr, int x, int y) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));
	if (window) {
		window->data.x = static_cast<uint32_t>(x);
		window->data.y = static_cast<uint32_t>(y);
	}
}

void Window::WindowSizeCallback(GLFWwindow* windowPtr, int width, int height) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));
	if (window) {
		window->data.w = static_cast<uint32_t>(width);
		window->data.h = static_cast<uint32_t>(height);
	}
}

void Window::FramebufferSizeCallback(GLFWwindow* windowPtr, int width, int height) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowPtr));
	if (window) {
		window->data.fw = static_cast<uint32_t>(width);
		window->data.fh = static_cast<uint32_t>(height);
		window->data.fc = true;
	}
}