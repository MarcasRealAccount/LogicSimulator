#pragma once

#include "Rendering/Renderer.h"

class Entity;
struct OpenGLMeshData;
class OpenGLMaterialData;
class OpenGLShaderData;

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer(Window* window);

	virtual RendererType GetRendererType() const override;

	virtual void SetWindowHints() override;

protected:
	virtual void InitRenderer() override;
	virtual void DeInitRenderer() override;
	virtual void RenderScene(Scene* scene, CameraComponent* camera) override;

private:
	void RenderEntity(Entity* entity, CameraComponent* camera);
	void RenderMeshWithMaterial(OpenGLMeshData* meshData, OpenGLMaterialData* materialData);
	void RenderMesh(OpenGLMeshData* meshData);

	void PreMaterial(OpenGLMaterialData* materialData);
	void PostMaterial(OpenGLMaterialData* materialData);
};