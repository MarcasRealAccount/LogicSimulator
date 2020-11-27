#include "Rendering/Apis/OpenGL/OpenGLRenderer.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh2D.h"
#include "Rendering/Apis/OpenGL/Shader/OpenGLMaterialData.h"
#include "Rendering/Apis/OpenGL/Shader/OpenGLShaderData.h"
#include "Window/Window.h"
#include "ECS/Scene.h"
#include "ECS/Entity.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/MaterialComponent.h"
#include "ECS/Components/TransformComponent.h"

#include "Rendering/Apis/OpenGL/GLHelpers.h"

OpenGLRenderer::OpenGLRenderer(Window* window)
	: Renderer(window) {}

RendererType OpenGLRenderer::GetRendererType() const {
	return RendererType::OPENGL;
}

void OpenGLRenderer::SetWindowHints() {
	this->window->WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	this->window->WindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
}

void OpenGLRenderer::InitRenderer() {
	this->window->MakeContextCurrent();
	Window::SwapInterval(0);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		throw std::exception("Failed to initialize GLAD");
	}
	glSetupDebugMessenger();
}

void OpenGLRenderer::DeInitRenderer() {

}

void OpenGLRenderer::RenderScene(Scene* scene, CameraComponent* camera) {
	if (camera) {
		glViewport(0, 0, camera->width, camera->height);
		glClearColor(camera->clearColor.x, camera->clearColor.y, camera->clearColor.z, camera->clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		std::vector<MeshComponent*> meshComponents = scene->GetAllComponents<MeshComponent>();
		for (MeshComponent* comp : meshComponents) {
			RenderEntity(comp->GetParent(), camera);
		}

		glfwSwapBuffers(GetNativeWindowHandle());
	}
}

void OpenGLRenderer::RenderEntity(Entity* entity, CameraComponent* camera) {
	MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
	MaterialComponent* materialComponent = entity->GetComponent<MaterialComponent>();
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	if (meshComponent) mesh = meshComponent->mesh;
	if (materialComponent) material = materialComponent->material;
	if (material) {
		if (mesh) {
			TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
			if (transformComponent) {
				auto transformationMatrix = material->GetUniform<glm::fmat4>("transformationMatrix");
				if (transformationMatrix) transformationMatrix->value = transformComponent->GetTransformMatrix();
			}
			auto projectionViewMatrix = material->GetUniform<glm::fmat4>("projectionViewMatrix");
			if (projectionViewMatrix) projectionViewMatrix->value = camera->GetProjectionViewMatrix();
			auto time = material->GetUniform<float>("time");
			if (time) time->value = static_cast<float>(glfwGetTime());

			RenderMeshWithMaterial(GetMeshData<OpenGLMeshData>(mesh), GetMaterialData<OpenGLMaterialData>(material));
		}
	} else if (mesh) {
		RenderMesh(GetMeshData<OpenGLMeshData>(mesh));
	}
}

void OpenGLRenderer::RenderMeshWithMaterial(OpenGLMeshData* meshData, OpenGLMaterialData* materialData) {
	PreMaterial(materialData);
	RenderMesh(meshData);
	PostMaterial(materialData);
}

void OpenGLRenderer::RenderMesh(OpenGLMeshData* meshData) {
	Mesh* mesh = meshData->GetMesh<Mesh>();
	if (mesh->renderMode == RenderMode::POINTS) glPointSize(mesh->lineWidth);
	else glLineWidth(mesh->lineWidth);

	glBindVertexArray(meshData->GetVAO());
	if (meshData->HasIndices()) glDrawElements(meshData->GetRenderMode(), meshData->bufferSize, GL_UNSIGNED_INT, 0);
	else glDrawArrays(meshData->GetRenderMode(), 0, meshData->bufferSize);
	glBindVertexArray(0);

	glPointSize(1);
	glLineWidth(1);
}

void OpenGLRenderer::PreMaterial(OpenGLMaterialData* materialData) {
	Material* material = materialData->GetMaterial<Material>();
	if (material->cullMode.enabled) {
		glEnable(GL_CULL_FACE);
		glCullFace(materialData->GetCullFace());
	}

	if (material->depthTest) glEnable(GL_DEPTH_TEST);

	if (material->blendFunc.enabled) {
		glEnable(GL_BLEND);
		glBlendFunc(materialData->GetSrcBlendFunc(), materialData->GetDstBlendFunc());
	}

	if (material->polygonMode.enabled) glPolygonMode(materialData->GetPolygonModeFace(), materialData->GetPolygonMode());

	OpenGLShaderData* shader = GetShaderData<OpenGLShaderData>(materialData);
	if (shader) {
		shader->Start();
		materialData->SetAllUniforms();
	}
}

void OpenGLRenderer::PostMaterial(OpenGLMaterialData* materialData) {
	OpenGLShaderData* shader = GetShaderData<OpenGLShaderData>(materialData);
	if (shader) shader->Stop();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}