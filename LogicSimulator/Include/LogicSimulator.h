#pragma once

#include "Utilities/Logger.h"
#include "Window/Window.h"

class Renderer;
class Scene;
class Material;

class LogicSimulator {
public:
	void Init();
	void Run();
	void DeInit();

private:
	Window window;
	Renderer* renderer = nullptr;

	Scene* scene = nullptr;
	Material* mat = nullptr;

	Logger logger{ "LogicSimulator" };
};