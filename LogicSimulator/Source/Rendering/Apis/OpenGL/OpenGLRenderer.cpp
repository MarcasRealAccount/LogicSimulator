#include "Rendering/Apis/OpenGL/OpenGLRenderer.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh2D.h"
#include "Rendering/Apis/OpenGL/Shader/OpenGLMaterialData.h"
#include "Rendering/Apis/OpenGL/Shader/OpenGLShaderData.h"
#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"

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

void OpenGLRenderer::RenderScene(Scene* scene, Camera* camera) {
	if (camera) {
		glViewport(0, 0, camera->width, camera->height);
		glClearColor(camera->clearColor.x, camera->clearColor.y, camera->clearColor.z, camera->clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto entity : scene->GetEntities()) RenderEntity(entity, camera);

		glfwSwapBuffers(GetNativeWindowHandle());
	}
}

void OpenGLRenderer::RenderEntity(Entity* entity, Camera* camera) {
	Scene* scene = entity->GetScene();
	if (!scene) return;

	Mesh* mesh = entity->GetMesh();
	Material* material = entity->GetMaterial();
	if (material) {
		if (mesh) {
			auto transformationMatrix = material->GetUniform<glm::fmat4>("transformationMatrix");
			if (transformationMatrix) transformationMatrix->value = entity->transform.GetTransformMatrix();
			auto projectionViewMatrix = material->GetUniform<glm::fmat4>("projectionViewMatrix");
			if (projectionViewMatrix) projectionViewMatrix->value = *camera->GetProjectionViewMatrix();
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