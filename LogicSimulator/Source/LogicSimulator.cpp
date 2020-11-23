#include "LogicSimulator.h"
#include "Utilities/Config/ConfigManager.h"
#include "Window/Input/InputHandler.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Rendering/Mesh/Primitives/Primitives.h"

void LogicSimulator::Init() {
	Logger::Init();
	InputHandler::Init();

	this->window.SetSize(1280, 720);
	this->renderer = Renderer::GetRenderer(RendererType::OPENGL, &this->window);
	this->renderer->SetWindowHints();
	if (!this->window.Create()) throw std::exception("Failed to create GLFW window");
	this->renderer->Init();

	InputHandler::SetWindow(&this->window);

	this->scene = new Scene();
	this->cam = new OrthoCamera({ 5 });
	this->cam->transform.position.x = 0.5f;
	this->scene->AttachEntity(this->cam);
	this->scene->SetMainCamera(this->cam);
	this->mat = new Material();
	this->mat->SetShader(Shader::GetShader("shader"));
	this->entity = new MeshedEntity(const_cast<Mesh2D*>(&GetRect2DPrimitive()), this->mat);
	this->scene->AttachEntity(this->entity);

	Scene::SetMainScene(this->scene);
}

void LogicSimulator::Run() {
	while (!this->window.ShouldWindowClose()) {
		Window::PollEvents();
	}
}

void LogicSimulator::DeInit() {
	if (this->renderer) this->renderer->DeInit();
	this->window.Destroy();
	if (this->entity) delete entity;
	if (this->mat) delete mat;
	if (this->cam) delete cam;
	if (this->scene) delete scene;
	ConfigManager::SaveConfigs();
	Logger::DeInit();
}