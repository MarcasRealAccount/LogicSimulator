#pragma once

#include "Utilities/Logger.h"
#include "Window/Window.h"

class Renderer;
struct Scene;
class Entity;
class Camera;
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
	Camera* cam = nullptr;
	Entity* entity = nullptr;
	Material* mat = nullptr;

	Logger logger{ "LogicSimulator" };
};