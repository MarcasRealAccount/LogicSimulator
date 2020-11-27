#pragma once

#include "Rendering/RendererType.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Shader/Material.h"
#include "Rendering/Shader/Shader.h"
#include "Utilities/Logger.h"

#include <stdint.h>
#include <queue>
#include <mutex>
#include <thread>

struct GLFWwindow;
class Window;
struct Mesh2D;
struct MeshData;
class Material;
class MaterialData;
class Shader;
class ShaderData;
class Scene;
class CameraComponent;

class Renderer {
public:
	Renderer(Window* window);
	virtual ~Renderer() = default;

	virtual RendererType GetRendererType() const = 0;

	virtual void SetWindowHints() = 0;
	void Init();
	void DeInit();

	void AddSceneToRender(Scene* scene, CameraComponent* camera);
	void RemoveSceneToRender(Scene* scene, CameraComponent* camera);

	virtual bool IsMeshDataUsable(MeshData* meshData);
	virtual bool IsMaterialDataUsable(MaterialData* materialData);
	virtual bool IsShaderDataUsable(ShaderData* shaderData);

protected:
	virtual void InitRenderer() = 0;
	virtual void DeInitRenderer() = 0;
	virtual void RenderScene(Scene* scene, CameraComponent* camera) = 0;

	GLFWwindow* GetNativeWindowHandle() const;

	template <typename T>
	T* GetMeshData(Mesh* mesh) {
		return reinterpret_cast<T*>(mesh->GetMeshData(this));
	}

	template <typename T>
	T* GetMaterialData(Material* material) {
		return reinterpret_cast<T*>(material->GetMaterialData(this));
	}

	template <typename T>
	T* GetShaderData(MaterialData* materialData) {
		return GetShaderData<T>(materialData->GetMaterial<Material>()->GetShader());
	}

	template <typename T>
	T* GetShaderData(Shader* shader) {
		return reinterpret_cast<T*>(shader->GetShaderData(this));
	}

private:
	void RenderThreadFunc();

public:
	static Renderer* GetRenderer(RendererType rendererType, Window* window);

protected:
	std::thread* renderThread = nullptr;
	Window* window;
	bool running = false;

	std::vector<std::pair<Scene*, CameraComponent*>> scenesToRender;
	std::queue<std::pair<Scene*, CameraComponent*>> scenesToAddToRender;
	std::queue<std::pair<Scene*, CameraComponent*>> scenesToRemoveFromRender;
	std::mutex lock;

private:
	static Logger logger;
};