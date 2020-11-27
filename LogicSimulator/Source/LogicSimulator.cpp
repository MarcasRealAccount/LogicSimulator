#include "LogicSimulator.h"
#include "Utilities/Config/ConfigManager.h"
#include "Window/Input/InputHandler.h"
#include "Rendering/Renderer.h"
#include "ECS/Scene.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/MaterialComponent.h"
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

	Entity* cam = this->scene->AddChild<Entity>();
	cam->AddComponent<CameraComponent>();
	this->scene->SetMainCamera(cam);
	TransformComponent* camTransform = cam->GetComponent<TransformComponent>();
	camTransform->position.x = 0.5f;

	this->mat = new Material();
	this->mat->SetShader(Shader::GetShader("shader"));

	Entity* entity = this->scene->AddChild<Entity>();
	entity->AddComponent<MeshComponent>(const_cast<Mesh2D*>(&GetRect2DPrimitive()));
	entity->AddComponent<MaterialComponent>(this->mat);

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
	if (this->mat) delete mat;
	if (this->scene) delete scene;
	ConfigManager::SaveConfigs();
	Logger::DeInit();
}