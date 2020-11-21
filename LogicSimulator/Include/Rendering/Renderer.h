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
struct Scene;
class Camera;

class Renderer {
public:
	Renderer(Window* window);
	virtual ~Renderer() = default;

	virtual RendererType GetRendererType() const = 0;

	virtual void SetWindowHints() = 0;
	void Init();
	void DeInit();

	void AddSceneToRender(Scene* scene, Camera* camera);
	void RemoveSceneToRender(Scene* scene, Camera* camera);

	virtual bool IsMeshDataUsable(MeshData* meshData);
	virtual bool IsMaterialDataUsable(MaterialData* materialData);
	virtual bool IsShaderDataUsable(ShaderData* shaderData);

protected:
	virtual void InitRenderer() = 0;
	virtual void DeInitRenderer() = 0;
	virtual void RenderScene(Scene* scene, Camera* camera) = 0;

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

	std::vector<std::pair<Scene*, Camera*>> scenesToRender;
	std::queue<std::pair<Scene*, Camera*>> scenesToAddToRender;
	std::queue<std::pair<Scene*, Camera*>> scenesToRemoveFromRender;
	std::mutex lock;

private:
	static Logger logger;
};