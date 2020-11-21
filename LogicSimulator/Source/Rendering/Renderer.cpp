#include "Rendering/Renderer.h"
#include "Rendering/Apis/OpenGL/OpenGLRenderer.h"
#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

Renderer::Renderer(Window* window)
	: window(window) {}

void Renderer::Init() {
	this->running = true;
	this->renderThread = new std::thread(&Renderer::RenderThreadFunc, this);
}

void Renderer::DeInit() {
	running = false;
	{
		std::unique_lock<std::mutex> mlock(this->lock);
		if (this->renderThread) this->renderThread->join();
		delete this->renderThread;
	}
}

void Renderer::AddSceneToRender(Scene* scene, Camera* camera) {
	std::unique_lock<std::mutex> mlock(this->lock);
	this->scenesToAddToRender.push({ scene, camera });
}

void Renderer::RemoveSceneToRender(Scene* scene, Camera* camera) {
	std::unique_lock<std::mutex> mlock(this->lock);
	this->scenesToRemoveFromRender.push({ scene, camera });
}

bool Renderer::IsMeshDataUsable(MeshData* meshData) {
	return meshData->GetRendererType() == GetRendererType();
}

bool Renderer::IsMaterialDataUsable(MaterialData* materialData) {
	return materialData->GetRendererType() == GetRendererType();
}

bool Renderer::IsShaderDataUsable(ShaderData* shaderData) {
	return shaderData->GetRendererType() == GetRendererType();
}

GLFWwindow* Renderer::GetNativeWindowHandle() const {
	return this->window->windowPtr;
}

void Renderer::RenderThreadFunc() {
	InitRenderer();
	while (this->running) {
		{	// Lock the queues and add or remove the new scenes so the renderer can render the new scenes or stop rendering old scenes.
			std::unique_lock<std::mutex> mlock(this->lock);
			while (!this->scenesToAddToRender.empty()) {
				auto& add = this->scenesToAddToRender.front();
				this->scenesToRender.push_back(add);
				this->scenesToAddToRender.pop();
			}
			while (!this->scenesToRemoveFromRender.empty()) {
				auto& remove = this->scenesToRemoveFromRender.front();
				auto itr = this->scenesToRender.begin();
				while (itr != this->scenesToRender.end()) {
					if (itr->first == remove.first && itr->second == remove.second) {
						this->scenesToRender.erase(itr);
						break;
					}
					itr++;
				}
				this->scenesToRemoveFromRender.pop();
			}
		}

		Scene* mainScene = Scene::GetMainScene();
		if (mainScene) {
			Camera* mainCamera = mainScene->GetMainCamera();
			if (mainCamera) {
				mainCamera->width = this->window->GetData().fw;
				mainCamera->height = this->window->GetData().fh;
				RenderScene(mainScene, mainCamera);
			}
		}

		for (auto& scene : this->scenesToRender) {
			RenderScene(scene.first, scene.second);
		}
	}
	DeInitRenderer();
}

Renderer* Renderer::GetRenderer(RendererType rendererType, Window* window) {
	switch (rendererType) {
	case RendererType::OPENGL:
		return new OpenGLRenderer(window);
	default:
		return nullptr;
	}
}