#include "Rendering/Renderer.h"
#include "Rendering/Apis/OpenGL/OpenGLRenderer.h"
#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

Logger Renderer::logger("Renderer");

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
	try {
		InitRenderer();
	} catch (std::exception e) {
		Renderer::logger.LogError("Exception was thrown when calling InitRenderer()!\n%s", e.what());
		this->running = false;
	}
	while (this->running) {
		try {	// Lock the queues and add or remove the new scenes so the renderer can render the new scenes or stop rendering old scenes.
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
		} catch (std::exception e) {
			Renderer::logger.LogError("Exception was thrown when either adding or removing a scene from the renderer!\n%s", e.what());
		}

		try {
			Scene* mainScene = Scene::GetMainScene();
			if (mainScene) {
				Camera* mainCamera = mainScene->GetMainCamera();
				if (mainCamera) {
					mainCamera->width = this->window->GetData().fw;
					mainCamera->height = this->window->GetData().fh;
					std::unique_lock<std::mutex> mlock(mainScene->lock);
					RenderScene(mainScene, mainCamera);
				}
			}
		} catch (std::exception e) {
			Renderer::logger.LogError("Exception was thrown when rendering the main scene!\n%s", e.what());
		}

		for (auto& scene : this->scenesToRender) {
			try {
				std::unique_lock<std::mutex> mlock(scene.first->lock);
				RenderScene(scene.first, scene.second);
			} catch (std::exception e) {
				Renderer::logger.LogError("Exception was thrown when rendering scene {%p} using camera {%p}!\n%s", scene.first, scene.second, e.what());
			}
		}
	}
	try {
		DeInitRenderer();
	} catch (std::exception e) {
		Renderer::logger.LogError("Exception was thrown when calling DeInitRenderer()!\n%s", e.what());
	}
}

Renderer* Renderer::GetRenderer(RendererType rendererType, Window* window) {
	switch (rendererType) {
	case RendererType::OPENGL:
		return new OpenGLRenderer(window);
	default:
		return nullptr;
	}
}