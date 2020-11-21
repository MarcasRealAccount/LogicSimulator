#pragma once

#include "Rendering/Shader/Material.h"
#include "Rendering/Apis/OpenGL/GLHelpers.h"

class OpenGLRenderer;

class OpenGLMaterialData : public MaterialData {
public:
	OpenGLMaterialData(Material* material);

	virtual RendererType GetRendererType() const override;
	virtual void CleanUp() override;

	GLenum GetCullFace() const;
	GLenum GetSrcBlendFunc() const;
	GLenum GetDstBlendFunc() const;
	GLenum GetPolygonModeFace() const;
	GLenum GetPolygonMode() const;

	void SetAllUniforms();

	friend OpenGLRenderer;

private:
	static GLenum GetGLCullFace(TriangleFace face);
	static GLenum GetGLBlendFunc(BlendFunc blendFunc);
	static GLenum GetGLPolygonMode(PolygonMode polygonMode);
};